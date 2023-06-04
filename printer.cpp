//
// Created by Osayamen on 2/25/2023.
//

#include <string>
#include <iomanip>
#include <sstream>
#include <unordered_set>
#include <deque>
#include <unordered_map>
#include <iostream>

#include "utility/utility.h"
#include "printer/printer.h"
#include "printer/communicator/communicator.h"
#include "printer/printer_manager/printer_manager.h"
#include "printer_utility/printer_utility.h"

std::string paper_name = "printer.out";
std::unordered_map<int, std::unordered_set<int>> spool_files; // CID -> set of its processes.
std::string spool_f_name = "spool_";
std::deque<PrinterOpDetails> printer_ops;

int NC;
int MQS;
/*std::string ip_addr;*/
/*int port;*/
int CQS;
int PT;
bool printer_terminate_flag;

void printer_init(){
    atomically_print_to_stdout("Printer Process starting...");
    spool_files = std::unordered_map<int, std::unordered_set<int>>();
    printer_ops = std::deque<PrinterOpDetails>();

    if(exists(paper_name) && remove(paper_name.c_str())){
        perror("Error initializing the printer");
        exit(0);
    }

    std::FILE* paper = fopen (paper_name.c_str(),"a+");
    if(paper != nullptr){
        fclose(paper);
    }
    else{
        perror(paper_name.append(" failed to open").c_str());
        exit(0);
    }
}

void printer_init_spool(int computer_id, int processID){
    std::string f_name = get_spool_fname(computer_id, processID);
    std::FILE* sp;
    sp = fopen (f_name.c_str(),"a+");
    if(sp != nullptr){
        if(!spool_files.count(computer_id)){
            spool_files.emplace(computer_id, std::unordered_set<int>());
        }
        spool_files.at(computer_id).insert(processID);
        fclose(sp);
    }
    else{
        perror(f_name.append(" failed to open").c_str());
        exit(processID);
    }
}

std::string get_process_name(int comp_id, int pid){
    return std::to_string(comp_id).append("_").append(std::to_string(pid));
}

void printer_end_spool(int comp_id, int pid){
    std::FILE* spool_file = fopen(get_spool_fname(comp_id, pid).c_str(), "a+");
    std::FILE* paper = fopen(paper_name.c_str(), "a+");
    std::string title = get_indented_str("Process " + get_process_name(comp_id, pid), 19);

    fputs(generate_header().c_str(), paper);
    fputs(title.c_str(), paper);
    fputs(generate_header().c_str(), paper);

    append_file(spool_file, paper);

    fclose(spool_file);
    fclose(paper);

    if(remove(get_spool_fname(comp_id, pid).c_str())){
        perror(get_spool_fname(comp_id, pid).append(" failed to delete").c_str());
    }
    spool_files.at(comp_id).erase(pid);
    atomically_print_to_stdout("Completed printing for finished Process " + get_process_name(comp_id, pid));
}

void printer_end_spool(int comp_id, int pid, const std::string& addendum){ //switch to closing here
    std::FILE* spool_file = fopen(get_spool_fname(comp_id, pid).c_str(), "a+");
    std::FILE* paper = fopen(paper_name.c_str(), "a+");
    std::string title = get_indented_str("Process " + get_process_name(comp_id, pid), 19);

    fputs(generate_header().c_str(), paper);
    fputs(title.c_str(), paper);
    fputs(generate_header().c_str(), paper);

    append_file(spool_file, paper, addendum);

    fclose(spool_file);
    fclose(paper);

    if(remove(get_spool_fname(comp_id, pid).c_str())){
        perror(get_spool_fname(comp_id, pid).append(" failed to delete").c_str());
    }
    atomically_print_to_stdout("Completed printing for unfinished Process " + get_process_name(comp_id, pid));
}

void printer_print(int comp_id, int pid, const std::string& buf){// assume buf has a newline at the end.
    std::FILE* spool_file = fopen(get_spool_fname(comp_id, pid).c_str(), "a+");
    fputs(buf.c_str(), spool_file);
    fclose(spool_file); // needed to persist the data to the file.
}

void printer_dump_spool(int comp_id){
    std::stringstream dump_stream;
    dump_stream << generate_header();
    dump_stream << std::setw(17) << "Printer Dump" << std::endl;
    dump_stream << generate_header();

    dump_stream << "Index: PID" << std::endl;
    int i = 0;
    if(spool_files.count(comp_id)){
        for(const auto& pid : spool_files.at(comp_id)){
            dump_stream << i << ": " << pid << std::endl;
            i++;
        }
    }
    std::string  dump_str = dump_stream.str();
    atomically_print_to_stdout(dump_str);
}

void printer_main(){
    sem_wait(&print_comm_init);
    printer_init();

    atomically_print_to_stdout("Printer has started!");

    sem_post(&manager_init);
    sem_post(&print_comm_init);
    sem_destroy(&print_comm_init);

    while(!printer_terminate_flag){
        sem_wait(&sync_pc);

        message_queues_consume();
        auto num_ops = printer_ops.size();
        auto num_waits = num_ops - 1;

        while(!printer_ops.empty()){
            execute_printer_op(printer_ops.front());
            PrinterOpDetails d = printer_ops.front();
            printer_ops.pop_front();
            if(d.op == TERMINATE){
                signal_termination(d.c_id);
            }
        }

        while(num_waits > 0){
            num_waits--;
            sem_wait(&sync_pc);
        }
    }
}

void printerOps_queue_insert(const std::vector<int>& ops, int communicator_id){
    PrinterOpDetails p = {
            getPrinterOp(ops.at(0)),
            ops.at(1),
            ops.at(2),
            ops.at(3),
            communicator_id
    };
    printer_ops.push_back(p);
}

void execute_printer_op(PrinterOpDetails op_details){
    int printer_op = op_details.op;
    int comp_id = op_details.computer_id;
    int pid = op_details.pid;
    int buf = op_details.buf;

    auto operation = getPrinterOp(printer_op);
    switch(operation){
        case INIT_SPOOL:{
            printer_init_spool(comp_id, pid);
            break;
        }
        case END_SPOOL:{
            printer_end_spool(comp_id, pid);
            break;
        }
        case PRINT:{
            printer_print(comp_id, pid, std::to_string(buf).append("\n"));
            break;
        }
        case TERMINATE:
            printer_terminate(comp_id);
            break;
        case DUMP_SPOOL:
            printer_dump_spool(comp_id);
            break;
        default:{
            std::string err = "Wrong PrinterOp received";
            err.append(std::to_string(operation));
            perror(err.c_str());
            exit(NOOP);
        }
    }
}

void printer_terminate(int comp_id){
    std::string addendumB = " did not complete before termination";
    if(spool_files.count(comp_id)){
        for(const auto& pid : spool_files.at(comp_id)){
            std::string addendum = "Process " + get_process_name(comp_id, pid) + addendumB;
            printer_end_spool(comp_id, pid, addendum);
        }
        spool_files.erase(comp_id);
    }
}

std::string get_spool_fname(int cid, int pid){
    return spool_f_name + std::to_string(cid).append("_").append(std::to_string(pid)) + ".txt";
}

void load_printer_parameters(int skip){
    char fname[20] = "config.txt";

    FILE *config_file = fopen(fname, "r");
    if (config_file == nullptr || feof(config_file)) {
        perror("Invalid Configuration file");
    }

    else{
        printer_terminate_flag = false;
        std::string line;

        std::tie(std::ignore, line) = get_line(config_file);
        ip_addr = line; // obtains printer's IP Address from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        port = std::stoi(line); // obtains printer's port from config.sys file.

        for(int i = 0; i < skip; ++i){
            get_line(config_file); // skip M, TQ
        }

        std::tie(std::ignore, line) = get_line(config_file);
        PT = std::stoi(line); // obtains NC from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        NC = std::stoi(line); // obtains NC from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        CQS = std::stoi(line); // obtains CQS from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        MQS = std::stoi(line); // obtains MQS from config.sys file.
    }
}


/*int main(){
    load_printer_parameters(2);
    printer_manager(NC, CQS, MQS);
    return 0;
}*/


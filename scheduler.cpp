//
// Created by Osayamen on 2/25/2023.
//

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "computer/print.h"
#include "utility/utility.h"
#include "scheduler/queue.h"
#include "scheduler/scheduler.h"

std::unordered_map<int, PCB> PCBs;
int initial_PID;
PCB idle_prog;
int idle_prog_PID = 0;
PCB running_pcb;
int current_mem_instr_time;
MemoryMetadata current_process_metadata;
std::mutex mtx;

const PCB NULL_PCB = {
        idle_prog_PID
};
int rq_levels = 4;

int get_and_update_PID(){
    return initial_PID++;
}

void process_init_PCBs(){
    PCBs = std::unordered_map<int, PCB>();
}

PCB process_init_PCB(std::string &fname, MemoryMetadata &metadata){
    REGS regs = REGS();
    PCB p = {
        get_and_update_PID(),
        regs,
        0,
        false,
        fname,
        0,
        metadata
    };
    PCBs.insert({p.PID, p});
    return p;
}

void process_dispose_PCB(int pid){
    PCBs.erase(pid);
}

void process_dump_PCB(){
    std::stringstream dump_stream;
    dump_stream << generate_header();
    dump_stream << std::setw(17) <<"PCB Dump" << std::endl;
    dump_stream << generate_header();

    dump_stream << "Index: [ Filename:XXXXX, PID:#, PC:#, IR0:#, IR1:#, AC:#, MAR:#, MBR:# ]" << std::endl;
    int i  = 0;
    for(const auto& pcb : PCBs){
        PCB p = pcb.second;
        dump_stream << i << ": [";
        dump_stream << "Filename:" << p.file_name << ", ";
        dump_stream << "PID:" << p.PID << ", ";
        dump_stream << "PC:" << p.cpuRegisters.PC << ", ";
        dump_stream << "IR0:" << p.cpuRegisters.IR0 << ", ";
        dump_stream << "IR1:" << p.cpuRegisters.IR1 << ", ";
        dump_stream << "AC:" << p.cpuRegisters.AC << ", ";
        dump_stream << "MAR:" << p.cpuRegisters.MAR << ", ";
        dump_stream << "MBR:" << p.cpuRegisters.MBR << " ";
        dump_stream << "]" << std::endl;
        i++;
    }
    std::string  dump_str = dump_stream.str();
    atomically_print_to_stdout(dump_str);

}

void process_init_readyQ(){
    queue_init(rq_levels);
}

void process_insert_readyQ(PCB &p){
    mtx.lock();
    queue_insert(p);
    mtx.unlock();
}

PCB process_fetch_readyQ(){
    PCB p = queue_fetch();
    if(p == NULL_PCB) return idle_prog;
    p.finishedTQ = false;
    return p;
}

void process_dump_readyQ(){
    queue_dump();
}


void process_scheduler_init() {
    //initialize registers to idle program state
    registers = REGS();
    registers.AC = 1; // needs to be greater than zero.

    // load idle program
    std::string idle_fname = "prog_idle.txt";
    MemoryMetadata m = load_prog(const_cast<char*>(idle_fname.c_str()));

    idle_prog = PCB();
    idle_prog.PID = idle_prog_PID;
    idle_prog.cpuRegisters = registers;
    idle_prog.metadata = m;
    idle_prog.file_name = idle_fname;

    current_mem_instr_time = 0;
    process_init_PCBs();
    initial_PID = 1;

    current_process_metadata = idle_prog.metadata;
    running_pcb = PCB();
    process_init_readyQ();
}

void process_context_switch(PCB &currentPCB, PCB &newPCB){
    std::string ctx_switch_log = std::string("Switching from Process {PID->")
            .append(std::to_string(currentPCB.PID))
            .append(", PC->")
            .append(std::to_string(currentPCB.cpuRegisters.PC))
            .append("} ")
            .append("to Process {PID->")
            .append(std::to_string(newPCB.PID))
            .append(", PC->")
            .append(std::to_string(newPCB.cpuRegisters.PC))
            .append("}");

    PCB temp = currentPCB;
    currentPCB = std::move(newPCB);

    if(temp.PID != idle_prog_PID) {
        process_insert_readyQ(temp);
        atomically_print_to_stdout(ctx_switch_log);
    }
}

void process_submit(std::string& fname, MemoryMetadata& metadata){
    PCB p = process_init_PCB(fname, metadata);
    print_act({INIT_SPOOL, CID, p.PID},
              "print::init_spool",
              "Started spool for process " + std::to_string(p.PID));
    process_insert_readyQ(p);
}

void process_set_registers(REGS &regs){
    registers = regs;
}

void process_set_mem_metadata(MemoryMetadata &m){
    current_process_metadata = m;
}

void process_execute(){
    ExecutionStatus e = Executing; // initial state
    while (!terminateFlag){
        switch(e){
            case Exit:
                process_exit(running_pcb.PID);
                running_pcb = process_fetch_readyQ();
                break;
            case TQExpiration: {
                running_pcb.finishedTQ = true; // needed for the multi-level feedback queue.
                PCB ready_pcb = process_fetch_readyQ(); // Returns the idle program when the RQ is empty.
                if(ready_pcb != idle_prog){
                    process_context_switch(running_pcb, ready_pcb);
                }
                else if(running_pcb != idle_prog){ // Continue executing the user's program since the RQ is empty.
                    running_pcb.priority = std::min((rq_levels - 1), (running_pcb.priority + 1));
                }
                break;
            }
            case IODelay: {
                running_pcb.finishedTQ = false;
                PCB ready_pcb = process_fetch_readyQ();
                if(ready_pcb != idle_prog){
                    process_context_switch(running_pcb, ready_pcb);
                }
                break;
            }
            case Executing:
                running_pcb = process_fetch_readyQ();
                break;
        }
        current_mem_instr_time = running_pcb.mem_instr_delay;
        process_set_mem_metadata(running_pcb.metadata);
        process_set_registers(running_pcb.cpuRegisters);
        e = cpu_operation(running_pcb.PID);
        running_pcb.mem_instr_delay = current_mem_instr_time;
        if(running_pcb.PID != idle_prog_PID){
            running_pcb.cpuRegisters = registers;
            PCBs.at(running_pcb.PID) = running_pcb; //update state after execution.
        }
    }
    scheduler_terminate();
}

void process_exit(int pid){
    reclaim_memory(running_pcb.metadata.page_table);
    process_dispose_PCB(pid);
    print_act({END_SPOOL, CID, pid},
              "print::end_spool",
              "Ended spooling for process " + std::to_string(pid));
}

void scheduler_terminate(){
    atomically_print_to_stdout("Terminating scheduler...");
    PCBs.clear();
    print_act({TERMINATE, CID},
              "print::terminate",
              "Print & Printer Termination complete!");
    stop_print();
}

PCB get_running_PCB(){
    return running_pcb;
}

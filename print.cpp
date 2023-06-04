//
// Created by Osayamen on 2/25/2023.
//

#include <unistd.h>
#include <mutex>

#include "computer/print.h"
#include "utility/utility.h"
#include "connector/client_connector.h"
#include "computer/computer.h"

#define ERROR_STATUS (-1)

int comp_sock;
int commID;
int client_fd;
int cid;
int print_ACK = 1;
std::mutex dump_mtx;

// Multiple ACKs are necessary for synchronizing operations between print and the printer process.
void print_init_spool(int pid){
    PrinterOp op = INIT_SPOOL;
    int printerACK;
    std::string func_name = "init_spool";

    sendAll(comp_sock, op, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, cid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 2).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, pid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 3).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 4).c_str());
        exit(ERROR_STATUS);
    }

    atomically_print_to_stdout("Started spool for process " + std::to_string(pid));
}

void print_end_spool(int pid){
    PrinterOp op = END_SPOOL;
    int printerACK;
    std::string func_name = "end_spool";

    sendAll(comp_sock, op, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, cid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 2).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, pid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 3).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 4).c_str());
        exit(ERROR_STATUS);
    }

    atomically_print_to_stdout("Ended spooling for process " + std::to_string(pid));
}

void print_print(int pid, int buf){
    PrinterOp op = PRINT;
    int printerACK;
    std::string func_name = "print";

    sendAll(comp_sock, op, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, cid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 2).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, pid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 3).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, buf, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 4).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 5).c_str());
        exit(ERROR_STATUS);
    }
}

void print_terminate(){
    PrinterOp op = TERMINATE;
    int printerACK;
    std::string func_name = "print";

    sendAll(comp_sock, op, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, cid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 2).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 3).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 4).c_str());
        exit(ERROR_STATUS);
    }
    stop_print();
    atomically_print_to_stdout("Print & Printer Termination complete!");
}

void print_dump_spool(){
    dump_mtx.lock();
    PrinterOp op = DUMP_SPOOL;
    int printerACK;
    std::string func_name = "dump_spool";

    sendAll(comp_sock, op, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, cid, 0);
    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 2).c_str());
        exit(ERROR_STATUS);
    }

    readAll(comp_sock, &printerACK);
    if(printerACK != print_ACK){
        perror(failed_ack_error_message(commID, func_name, 3).c_str());
        exit(ERROR_STATUS);
    }
    dump_mtx.unlock();
}


void print_init(){
    // set up client socket connection
    atomically_print_to_stdout("Connecting to Printer Server...");
    cid = CID;
    std::tie (comp_sock, client_fd) = client_connect(port, ip_addr);
    int comm_ACK;

    readAll(comp_sock, &comm_ACK); // read their print_ACK
    if(comm_ACK != print_ACK){
        perror(failed_ack_error_message(commID,"print_init", 1).c_str());
        exit(ERROR_STATUS);
    }

    sendAll(comp_sock, print_ACK, 0); // send my print_ACK

    readAll(comp_sock, &commID);
    atomically_print_to_stdout("Connected!");
}

void stop_print(){
    close(client_fd);
}

std::string failed_ack_error_message(int id, const std::string& func_name, int phase){
    return std::string ("Failed to hear back from ")
            .append(std::to_string(id))
            .append(" in phase ")
            .append(std::to_string(phase))
            .append(" inside ")
            .append(func_name);
}


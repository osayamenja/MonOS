//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#include <string>
#include <iostream>
#include <thread>

#include "computer/computer.h"
#include "computer/print.h"
#include "memory/memory.h"
#include "scheduler/scheduler.h"
#include "shell/shell.h"
#include "utility/utility.h"

int memorySize;
int TQ;
int mem_delay;
bool terminateFlag;
std::string ip_addr;
int port;
int CID;
int WS;

void read_system_parameters(){
    char fname[20] = "config.txt";

    FILE *config_file = fopen(fname, "r");
    if (config_file == nullptr || feof(config_file)) {
        perror("Invalid Configuration file");
    }

    else{
        std::string line;

        std::tie(std::ignore, line) = get_line(config_file);
        ip_addr = line; // obtains printer's IP Address from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        port = std::stoi(line); // obtains printer's port from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        memorySize = std::stoi(line); // obtains M from config.sys file.

        std::tie(std::ignore, line) = get_line(config_file);
        TQ = std::stoi(line); // obtains time quantum from config.sys file.
        mem_delay = TQ; // simulated memory delay for MLFQ scheduling

        std::tie(std::ignore, line) = get_line(config_file);
        WS = std::stoi(line); // obtains working set size from config.sys file.
    }
}

void boot_system(char* arg){
    terminateFlag = false;
    CID = int(strtol(arg, nullptr, 10));
    read_system_parameters();
}

int main(int argc, char* argv[]) {
    std::thread::id main_id = std::this_thread::get_id();
    boot_system(argv[1]);

    load_init();
    mem_init(memorySize);
    process_scheduler_init();

    print_init();
    shell_init();

    // needed to ensure orderly exit of destroyed, detached shell thread.
    if(std::this_thread::get_id() == main_id) {
        process_execute();
    }
    return 0;
}

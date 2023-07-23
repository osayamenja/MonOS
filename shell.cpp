//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#include <iostream>
#include <unistd.h>
#include <thread>

#include "computer/computer.h"
#include "computer/print.h"
#include "scheduler/scheduler.h"
#include "utility/utility.h"
#include "memory/memory.h"
#include "shell/shell.h"

bool shouldTerminate;
void shell_main(){
    int command;
    int sleep_time = 10000;
    while(!shouldTerminate){
        atomically_print_to_stdout("Enter Command>");
        if(!(std::cin >> command)){
            break;
        }
        shell_command(command);
        usleep(sleep_time);
    }
}


void shell_init(){
    shouldTerminate = false;
    std::thread shell (shell_main);
    shell.detach();
}

void shell_command(int cmd){
    std::string prog_file_name;
    MemoryMetadata m{};
    switch (cmd) {
        case 0:
            shouldTerminate = true;
            terminateFlag = true;
            break;
        case 1:
            atomically_print_to_stdout("Input Program File>");
            std::cin >> prog_file_name;
            m = load_prog(const_cast<char*>(prog_file_name.c_str()));
            if(!m.isOutOfMemory){
                process_submit(prog_file_name, m);
                atomically_print_to_stdout("Submitted fname->"
                                           + prog_file_name + "}");
            }
            else{
                atomically_print_to_stdout("OutOfMemory: Release memory and retry");
            }
            break;
        case 2:
            shell_print_registers();
            break;
        case 3:
            shell_print_memory();
            break;
        case 4:
            process_dump_readyQ();
            break;
        case 5:
            process_dump_PCB();
            break;
        /*case 6:
            print_act({DUMP_SPOOL, CID}, "dump_spool");
            break;*/
        default:
            break;
    }
}


void shell_print_registers(){
    cpu_dump_registers();
}

void shell_print_memory(){
    mem_dump_secondary_memory();
}

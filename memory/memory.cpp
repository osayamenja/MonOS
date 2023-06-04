//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#include <iomanip>
#include <vector>
#include <sstream>

#include "../computer/computer.h"
#include "../scheduler/scheduler.h"
#include "../utility/utility.h"
#include "memory.h"

std::vector<int> Mem;
void mem_init(int M){
    Mem = std::vector<int>(M);
}
void mem_read(){
    registers.MBR = Mem.at(registers.MAR);
}
void mem_write(){
    Mem.at(registers.MAR)= registers.MBR;
}

void mem_dump_memory(){
    std::stringstream dump_stream;
    int len = ((current_process_metadata.instruction_num*2) + current_process_metadata.data_num);
    dump_stream << generate_header();

    // k is the number of words in memory.
    dump_stream << std::setw(21) << "Memory Dump: k = " << memorySize << std::endl;
    dump_stream << generate_header();

    dump_stream << "Address: Contents" << std::endl;

    for(int i = 0; i < len; ++i){
        int j = cpu_mem_address(i);
        dump_stream << j << ": " << Mem.at(j) << std::endl;
    }

    std::string  dump_str = dump_stream.str();
    atomically_print_to_stdout(dump_str);
}
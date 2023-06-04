//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#ifndef AIMUYO_OS_PROJECT_COMPUTER_H
#define AIMUYO_OS_PROJECT_COMPUTER_H

#include <cstdio>
#include <mutex>
#include "../cpu/cpu.h"
#include "../memory/load.h"

extern bool terminateFlag;

// Size of memory
extern int memorySize;

// Time quantum
extern int TQ;

// delay for mem IO instructions.
extern int mem_delay;

// CID
extern int CID;

struct PCB {
    int PID;
    REGS cpuRegisters;
    int priority;
    bool finishedTQ;
    std::string file_name;
    int mem_instr_delay;
    MemoryMetadata metadata;

    bool operator==(const PCB& rhs) const{
        return this->PID == rhs.PID;
    }
    bool operator!=(const PCB& rhs) const{
        return this->PID != rhs.PID;
    }
};

extern const PCB NULL_PCB;

// Computer functions
void boot_system(const char* arg);
void read_system_parameters();

#endif //AIMUYO_OS_PROJECT_COMPUTER_H

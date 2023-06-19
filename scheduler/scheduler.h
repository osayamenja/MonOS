//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_SCHEDULER_H
#define AIMUYO_OS_PROJECT_SCHEDULER_H

#include "../computer/computer.h"
#include "../memory/loader/load.h"

extern int current_mem_instr_time;
extern MemoryMetadata current_process_metadata;
extern REGS registers;

void process_init_PCBs();
PCB process_init_PCB(std::string &fname, MemoryMetadata &metadata);
void process_dispose_PCB(int pid);
void process_dump_PCB();
void process_init_readyQ();
void process_insert_readyQ(PCB &p);
PCB process_fetch_readyQ();
void process_dump_readyQ();
void process_context_switch(PCB &currentPCB, PCB &newPCB);
void process_scheduler_init();
void process_set_registers(REGS &regs);
void process_submit(std::string& fname, MemoryMetadata& m);
void process_execute();
void process_exit(int pid);
void scheduler_terminate();
PCB get_running_PCB();

void process_set_mem_metadata(MemoryMetadata &m);

#endif //AIMUYO_OS_PROJECT_SCHEDULER_H

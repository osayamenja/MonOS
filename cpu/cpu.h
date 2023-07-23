//
// Author:  Osayamen Aimuyo
// Created: 1/30/2023.
//

#ifndef AIMUYO_OS_PROJECT_CPU_H
#define AIMUYO_OS_PROJECT_CPU_H

struct REGS {
    int AC;
    int IR0;
    int IR1;
    int MAR;
    int MBR;
    int PC;
};

enum ExecutionStatus{
    Exit,
    TQExpiration,
    IODelay,
    Executing
};


void cpu_fetch_instruction();
void cpu_execute_instruction(int pid);
int cpu_mem_address(int m_addr);
ExecutionStatus cpu_operation(int pid);
void cpu_dump_registers();

#endif //AIMUYO_OS_PROJECT_CPU_H

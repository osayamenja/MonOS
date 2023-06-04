//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_PRINT_H
#define AIMUYO_OS_PROJECT_PRINT_H

#include <string>

void print_init();
void print_init_spool(int pid);
void print_end_spool(int pid);
void print_dump_spool();
void print_print(int pid, int buf);
void print_terminate();
void stop_print();

std::string failed_ack_error_message(int id, const std::string& func_name, int phase);
#endif //AIMUYO_OS_PROJECT_PRINT_H

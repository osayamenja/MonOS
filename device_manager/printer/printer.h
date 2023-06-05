//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_PRINTER_H
#define AIMUYO_OS_PROJECT_PRINTER_H

#include <string>
#include <semaphore.h>
#include <vector>
#include "../../computer/print.h"
#include "../../utility/utility.h"

extern sem_t sync_pc;

// Printing Time
extern int PT;

// Message Queue size
extern int MQS;

// Num of communicators
extern int NC;

// Connection Queue size
extern int CQS;

// printer termination
extern bool printer_terminate_flag;

struct PrinterOpDetails{
    PrinterOp op;
    int computer_id;
    int pid;
    int buf;
    int c_id; // communicator id
};

void printer_init();
void printer_init_spool(int computer_id, int pid);
void printer_end_spool(int computer_id, int pid);
void printer_dump_spool(int computer_id);
void printer_print(int computer_id, int pid, const std::string& buf);
void printer_terminate(int computer_id);
void printer_main();
void execute_printer_op(PrinterOpDetails op_details);
void printerOps_queue_insert(const std::vector<int>& ops, int communicator_id);


std::string get_spool_fname(int cid, int pid);

#endif //AIMUYO_OS_PROJECT_PRINTER_H

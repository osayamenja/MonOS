//
// Created by Osayamen on 4/1/2023.
//

#ifndef AIMUYO_OS_PROJECT_2_PRINTER_MANAGER_H
#define AIMUYO_OS_PROJECT_2_PRINTER_MANAGER_H

extern sem_t manager_init;
extern sem_t full;
extern sem_t empty;
extern sem_t cq_mtx;
extern sem_t print_comm_init;

void start_printer();
void printer_manager_init(int nc, int cqs, int mqs);
void printer_manager(int nc, int cqs, int mqs);
void connection_queue_insert(int conn);
int connection_queue_consume();

#endif //AIMUYO_OS_PROJECT_2_PRINTER_MANAGER_H

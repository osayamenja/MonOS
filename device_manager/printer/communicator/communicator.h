//
// Created by Osayamen on 4/1/2023.
//

#ifndef AIMUYO_OS_PROJECT_2_COMMUNICATOR_H
#define AIMUYO_OS_PROJECT_2_COMMUNICATOR_H

#include <mutex>
#include <semaphore.h>

extern std::vector<sem_t> mq_sync_semaphores;
extern std::vector<std::mutex> mq_mutexes;
extern std::vector<sem_t> terminate_semaphores;

void communicators_init(int nc, int mqs);
void communicator(int id);

void message_queues_consume();
void message_queues_insert(const std::string& m, int id);
void signal_termination(int id);


#endif //AIMUYO_OS_PROJECT_2_COMMUNICATOR_H

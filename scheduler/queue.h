//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_QUEUE_H
#define AIMUYO_OS_PROJECT_QUEUE_H

#include "scheduler.h"

void queue_init(int levels);
void queue_insert(PCB p);
PCB queue_fetch();
void queue_dump();

#endif //AIMUYO_OS_PROJECT_QUEUE_H

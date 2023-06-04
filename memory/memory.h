//
// Author:  Osayamen Aimuyo
// Created: 1/30/2023.
//

#ifndef AIMUYO_OS_PROJECT_MEMORY_H
#define AIMUYO_OS_PROJECT_MEMORY_H

#include <vector>
extern std::vector<int> Mem;

// Memory functions
void mem_init(int M);
void mem_read();
void mem_write();
void mem_dump_memory();
#endif //AIMUYO_OS_PROJECT_MEMORY_H

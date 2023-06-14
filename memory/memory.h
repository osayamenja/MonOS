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
int get_page_number(int virtual_address);
int get_page_offset(int virtual_address);
int get_frame_number(int virtual_address);
int get_physical_address(int page_no, int offset);
void mem_read();
void mem_write();
void mem_dump_memory();
#endif //AIMUYO_OS_PROJECT_MEMORY_H

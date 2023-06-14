//
// Created by Osayamen on 2/27/2023.
//

#ifndef AIMUYO_OS_PROJECT_LOAD_H
#define AIMUYO_OS_PROJECT_LOAD_H

#include <cstdio>
#include <vector>

const int instruction_width = 2;
const int pageSize = instruction_width;

struct MemoryMetadata{
    int instruction_num;
    int data_num;
    int base;
    bool isOutOfMemory;
    std::vector<int> page_table;
};

void load_init();
MemoryMetadata load_prog(char *fname);
void load_finish(std::FILE *f);
bool populate_memory(char str[]);

#endif //AIMUYO_OS_PROJECT_LOAD_H

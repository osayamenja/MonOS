//
// Created by Osayamen on 2/27/2023.
//

#ifndef AIMUYO_OS_PROJECT_LOAD_H
#define AIMUYO_OS_PROJECT_LOAD_H

#include <cstdio>

struct MemoryMetadata{
    int instruction_num;
    int data_num;
};

MemoryMetadata load_prog(char *fname, int base);
void load_finish(std::FILE *f);
int populate_memory(char str[], int index);

#endif //AIMUYO_OS_PROJECT_LOAD_H

//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#include <cstdio>
#include <cstring>
#include <string>
#include <tuple>

#include "memory/load.h"
#include "utility/utility.h"
#include "memory/memory.h"
#include "computer_utility/computer_utility.h"

int populate_memory(char str[], int index){
    char *word = strtok(str, " ");

    while(word != nullptr && strcmp(word, "\n") != 0){
        Mem.at(index++) = std::stoi(word); // load instruction into memory.
        word = strtok(nullptr, " ");
    }
    return index;
}

MemoryMetadata load_prog(char *fname, int base){
    FILE *programFile = fopen(fname, "r");
    MemoryMetadata m = MemoryMetadata();
    if (programFile == nullptr || feof(programFile)) {
        perror("Invalid User Program");
    }
    else{
        bool isEOF;
        std::string line;

        std::tie(std::ignore, line) = get_line(programFile); // obtains program metadata.
        std::vector<int> v = separate_spaced_str(const_cast<char *>(line.c_str()));

        m.instruction_num = v.at(0);
        m.data_num = v.at(1);
        int i = base;

        std::tie(isEOF, line) = get_line(programFile);
        while(!isEOF){
            i = populate_memory(const_cast<char*>(line.c_str()), i);
            std::tie(isEOF, line) = get_line(programFile);
        }
        load_finish(programFile);
    }
    return m;
}

void load_finish(std::FILE *f){
    fclose(f);
}

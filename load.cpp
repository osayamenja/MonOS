//
// Author:  Osayamen Aimuyo
// Created: 1/29/2023.
//

#include <cstdio>
#include <cstring>
#include <string>
#include <tuple>
#include <deque>
#include <cmath>

#include "memory/loader/load.h"
#include "utility/utility.h"
#include "memory/memory.h"
#include "computer/computer.h"

std::deque<int> free_pages;
int current_page_no;
int disk_index;

bool atEndOfPage(int index){
    return index % pageSize == 0;
}

bool populate_memory(char str[]){
    bool advanced_page = false;
    char *word = strtok(str, " ");

    while(word != nullptr && strcmp(word, "\n") != 0){
        Mem.at(disk_index++) = std::stoi(word); // load instruction into memory.
        word = strtok(nullptr, " ");

        if(atEndOfPage(disk_index)){
            current_page_no = free_pages.front();
            free_pages.pop_front();
            advanced_page = true;
        }
    }
    return advanced_page;
}

int pages_needed(int instructions, int data){
    return ceil(((instructions * instruction_width) + data) / (pageSize * 1.0));
}

void load_init(){
    for(int i = 0; i < memorySize; ++i)
        free_pages.push_back(i);

}

MemoryMetadata load_prog(char *fname){
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

        if(free_pages.size() < pages_needed(m.instruction_num, m.data_num)) {
            //TODO: Maybe retry after running processes finish instead of returning?
            m.isOutOfMemory = true;
            return m;
        }

        disk_index = free_pages.front();
        m.page_table.push_back(disk_index);
        free_pages.pop_front();

        std::tie(isEOF, line) = get_line(programFile);
        current_page_no = disk_index;
        while(!isEOF){
            if(populate_memory(const_cast<char*>(line.c_str()))){
                m.page_table.push_back(current_page_no);
            }
            std::tie(isEOF, line) = get_line(programFile);
        }
        load_finish(programFile);
    }
    return m;
}

void load_finish(std::FILE *f){
    fclose(f);
}

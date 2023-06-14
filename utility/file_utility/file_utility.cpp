//
// Created by Osayamen on 4/2/2023.
//

#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include "file_utility.h"
#include "../../device_manager/printer/printer.h"

void append_file(std::FILE* from, std::FILE* append_to){
    int c = fgetc(from);
    if (c == EOF) return;
    while(c != EOF){
        fputc(c, append_to);
        c = fgetc(from);
        if (c == '\n') usleep(PT); // sleep before writing new line
    }
}

void append_file(std::FILE* from, std::FILE* append_to, const std::string& addendum){
    int c = fgetc(from);
    if (c == EOF) return;
    while(c != EOF){
        fputc(c, append_to);
        c = fgetc(from);
        if (c == '\n') usleep(PT); // sleep before writing new line
    }
    usleep(PT); // sleep before adding addendum.
    fputs(addendum.c_str(), append_to);
    putc('\n', append_to);

}

bool exists(std::string& file){ // use the file status library to resident_set_check the existence of a file.
    struct stat file_info{};
    return !stat(file.c_str(), &file_info);
}

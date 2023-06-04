//
// Created by Osayamen on 4/2/2023.
//

#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "printer_utility.h"
#include "../printer/printer.h"

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

bool exists(std::string& file){ // use the file status library to check the existence of a file.
    struct stat file_info{};
    return !stat(file.c_str(), &file_info);
}

std::string get_indented_str(const std::string& str, int indent){
    std::stringstream dump_stream;
    dump_stream << std::setw(indent) << str << std::endl;
    return dump_stream.str();
}

PrinterOp getPrinterOp(int i){
    switch(i){
        case 0:
            return INIT_SPOOL;
        case 1:
            return END_SPOOL;
        case 2:
            return PRINT;
        case 3:
            return DUMP_SPOOL;
        case 4:
            return TERMINATE;
        default:
            return NOOP;
    }
}
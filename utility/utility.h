//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_UTILITY_H
#define AIMUYO_OS_PROJECT_UTILITY_H

#include <string>
#include <vector>
#include "../computer/print.h"

// Printer IP address
extern std::string ip_addr;

// Printer port
extern int port;

enum PrinterOp{
    INIT_SPOOL,
    END_SPOOL,
    PRINT,
    DUMP_SPOOL,
    TERMINATE,
    NOOP
};

enum ACK{
    PRINTER_ACK
};

//Utility function
std::string generate_header();
std::tuple<bool, std::string> get_line(std::FILE* f);

void atomically_print_to_stdout(const std::string &s);
/*size_t sendAll(int fd, int buf, size_t n, int f);
size_t readAll(int fd, char *buffer, size_t n);*/
size_t sendAll(int fd, int buf, int f);
size_t readAll(int fd, int *output_buf);
//int get_int(const char *buffer);

#endif //AIMUYO_OS_PROJECT_UTILITY_H

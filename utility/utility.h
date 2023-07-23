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
    COMMUNICATOR_ACK = 1
};

//String utility functions
std::string generate_header();
std::tuple<bool, std::string> get_line(std::FILE* f);
std::string get_indented_str(const std::string& str, int indent);
std::vector<int> separate_spaced_str(char str[]);
std::vector<int> separate_str(char str[], const std::string& delim);
int conv_to_int(char* buffer);
size_t get_size(int i);

// std utility function
void atomically_print_to_stdout(const std::string &s);

// file descriptor I/O utility functions
int sendAny(int fd, char* data, ulong len, int f);
size_t readAny(int fd, char *data, ulong len, int f);
size_t sendInt(int fd, int buf, int f);
size_t readAndVerifyInt(int fd, char *data, ulong len, int f, int expected, const std::string& expectedName);

// socket utility
std::string build_msg(std::vector<int> args);

#endif //AIMUYO_OS_PROJECT_UTILITY_H

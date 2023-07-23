//
// Created by Osayamen on 2/25/2023.
//

#include <iostream>
#include <string>
#include <tuple>
#include <sys/socket.h>
#include <poll.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>

#include "utility.h"
#define ERROR_STATUS (-1)

std::string generate_header(){
    std::string str;

    for(int i = 0; i < 30; ++i)
        str.append("=");
    str.append("\n");
    return str;
}

std::tuple<bool, std::string> get_line(std::FILE* f){
    char c = static_cast<char>(fgetc(f));
    if(c == EOF) return std::make_tuple(true, "");
    std::string temp;
    while(c != '\n' && c != EOF){
        temp.push_back(c);
        c = static_cast<char>(fgetc(f));
    }
    return std::make_tuple(false, temp);
}

void atomically_print_to_stdout(const std::string &s){
    std::cout.write(s.c_str(), static_cast<int>(s.size())) << std::endl;
}

int sendAny(int fd, char* data, ulong len, int f){
    auto rem = len;
    while(rem > 0){
        auto rc = send(fd, data, rem, f);
        if(rc < 0) return -1;
        data += rc;
        rem -= rc;
    }
    return int(len);
}

size_t readAny(int fd, char *data, ulong len, int f){
    if(len <= 0) return len;

    struct pollfd poll_sock[1];
    poll_sock[0].fd = fd;
    poll_sock[0].events = POLLIN;

    auto rem = len;
    do{
        auto in = recv(fd, data, rem, f);
        if(in <= 0) return in;
        else {
            data += in;
            rem -= in;
        }
    } while(rem > 0 && poll(poll_sock, 1, 0) > 0);
    *data = '\0';
    return len;
}

size_t sendInt(int fd, int buf, int f){
    std::string conv = std::to_string(buf);
    char* conv_int = conv.data();

    return sendAny(fd, conv_int, conv.size(), f);
}

size_t readAndVerifyInt(int fd, char *data, ulong len, int f, int expected, const std::string& expectedName){
    auto r = readAny(fd, data, len, f);
    if(conv_to_int(data) != expected){
        perror(std::string("Failed to match ")
                       .append(expectedName)
                       .append(". Read ")
                       .append(std::string(data))
                       .append(". Expected ")
                       .append(std::to_string(expected)).data());
        exit(ERROR_STATUS);
    }
    return r;
}

std::string get_indented_str(const std::string& str, int indent){
    std::stringstream dump_stream;
    dump_stream << std::setw(indent) << str << std::endl;
    return dump_stream.str();
}

std::vector<int> separate_spaced_str(char str[]){
    return separate_str(str, " ");
}

std::vector<int> separate_str(char str[], const std::string& delim){
    char *word = strtok(str, delim.c_str());
    std::vector<int> a;
    while(word != nullptr && strcmp(word, "\n") != 0){
        a.push_back(std::stoi(word));
        word = strtok(nullptr, delim.c_str());
    }
    return a;
}

std::string build_msg(std::vector<int> args){
    std::string msg = std::to_string(args.at(0));

    for(int i = 1; i < args.size(); ++i){
        msg.append(", ").append(std::to_string(args.at(i)));
    }
    return msg;
}

int conv_to_int(char* buffer){
    return int(strtol(buffer, nullptr, 10));
}

size_t get_size(int i){
    return std::to_string(i).size();
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


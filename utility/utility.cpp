//
// Created by Osayamen on 2/25/2023.
//

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <tuple>
#include <sys/socket.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <vector>

#include "utility.h"

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

size_t sendAll(int fd, int buf, int f){
    auto conv_buf = htonl(buf);
    char *buffer = static_cast<char*>(static_cast<void*>(&conv_buf));
    auto rem = sizeof(conv_buf);

    while(rem > 0){
        auto gone = send(fd, buffer, rem, f);
        if(gone < 0) return gone;

        buffer += gone;
        rem -= gone;
    }
    return sizeof(conv_buf);
}

size_t readAll(int fd, int *output_buf)
{
    int output_int;
    char *data = static_cast<char*>(static_cast<void*>(&output_int));
    auto rem = sizeof(output_int);

    while (rem > 0) {
        auto in = recv(fd, data, rem, 0);
        if(in <= 0) return in;
        else {
            data += in;
            rem -= in;
        }
    }
    *output_buf = ntohl(output_int);
    return sizeof(output_int);
}

std::string get_indented_str(const std::string& str, int indent){
    std::stringstream dump_stream;
    dump_stream << std::setw(indent) << str << std::endl;
    return dump_stream.str();
}

std::vector<int> separate_spaced_str(char str[]){
    char *word = strtok(str, " ");
    std::vector<int>a;
    while(word != nullptr && strcmp(word, "\n") != 0){
        a.push_back(std::stoi(word));
        word = strtok(nullptr, " ");
    }
    return a;
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


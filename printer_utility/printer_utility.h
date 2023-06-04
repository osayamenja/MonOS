//
// Created by Osayamen on 4/2/2023.
//

#ifndef AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H
#define AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H


#include <string>
#include "../utility/utility.h"

void append_file(std::FILE* from, std::FILE* to);
void append_file(std::FILE* from, std::FILE* append_to, const std::string& addendum);
bool exists(std::string& file);
std::string get_indented_str(const std::string& str, int indent);
PrinterOp getPrinterOp(int i);


#endif //AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H

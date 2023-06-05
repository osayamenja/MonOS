//
// Created by Osayamen on 4/2/2023.
//

#ifndef AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H
#define AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H


#include <string>
#include "../utility.h"

void append_file(std::FILE* from, std::FILE* to);
void append_file(std::FILE* from, std::FILE* append_to, const std::string& addendum);
bool exists(std::string& file);



#endif //AIMUYO_OS_PROJECT_2_PRINTER_UTILITY_H

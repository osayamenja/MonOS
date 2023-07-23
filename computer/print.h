//
// Created by Osayamen on 2/25/2023.
//

#ifndef AIMUYO_OS_PROJECT_PRINT_H
#define AIMUYO_OS_PROJECT_PRINT_H

#include <string>
#include <vector>

void print_act(const std::vector<int>& action, const std::string& name, const std::string& success_msg = "");
void print_init();
void stop_print();

#endif //AIMUYO_OS_PROJECT_PRINT_H

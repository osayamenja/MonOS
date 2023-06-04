//
// Created by Osayamen on 4/2/2023.
//

#include <vector>
#include <string>
#include <cstring>
#include "computer_utility.h"
#include "../utility/utility.h"

void display_process_continuation_msg(int pid, int base){
    std::string s = "Continuing Execution of {PID->"
                    + std::to_string(pid) + ", Base->"
                    + std::to_string(base) + "}";

    atomically_print_to_stdout(s);
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

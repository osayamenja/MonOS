//
// Created by Osayamen on 4/2/2023.
//

#include <string>
#include "computer_utility.h"
#include "../utility.h"

void display_process_continuation_msg(int pid, const std::string& metadata){
    std::string s = "Continuing Execution of {PID->"
                    + std::to_string(pid) + ", Metadata->" + metadata + "}";

    atomically_print_to_stdout(s);
}

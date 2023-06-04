//
// Author:  Osayamen Aimuyo
// Created: 1/30/2023.
//

#ifndef AIMUYO_OS_PROJECT_SHELL_H
#define AIMUYO_OS_PROJECT_SHELL_H

void shell_init();
void shell_print_registers();
void shell_print_memory();
void shell_command (int cmd);

//main
void shell_main();

#endif //AIMUYO_OS_PROJECT_SHELL_H

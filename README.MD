# MonoOS
A monolithic OS with MLFQ process scheduling, on-demand paging, inter-process synchronization and communication and 
a device manager supporting network devices. 

On device management, MonoOS currently supports connections to a simulated printer. 

MonoOS also features a simulated CPU which executes user programs written as specified in [User Programs](#assembly).

## Assembly
Below is the assembly datasheet.

| OP Code | Instruction | Operand | Operation                                            |
|:-------:|:-----------:|:-------:|------------------------------------------------------|
|    1    |    load     |  word   | load integer word to AC register                     |
|    2    |    load     | address | load contents at address to AC                       |
|    3    |     add     | address | load contents to MBR and add MBR to AC               |
|    4    |     mul     | address | Similar to add instruction but for multiplication    |
|    5    |    store    | address | Store AC to address                                  |
|    6    |    ifgo     | address | If AC > 0, branch to address; else, next instruction |
|    7    |    print    |    0    | Print AC, operand is unused                          |
|    8    |    sleep    |    t    | Sleep for t microseconds                             |
|    9    |    shell    |  code   | Execute the shell command for given code             |
|    0    |    exit     |    0    | End of current user program                          |

### Shell
MonoOS provides an interactive shell that allows users to submit programs, terminate the OS, 
and view internal system state, such as register and memory contents, ready queue processes, etc.

| Code | Operation                                                                                  |
|:----:|--------------------------------------------------------------------------------------------|
|  0   | Terminate the OS                                                                           |
|  1   | Submit a new program. File name must be specified when prompted with "Input Program File>" |
|  2   | Dump state of registers                                                                    |
|  3   | Dump contents of memory pages for running process                                          |
|  4   | Dump PIDs of processes in the MLFQ                                                         |
|  5   | Dump all PCBs                                                                              |

Below are shell commands accessible as instructions by the simulated CPU.

| Code | Operation                                                       |
|:----:|-----------------------------------------------------------------|
|  2   | Dump state of registers                                         |
|  3   | Dump contents of memory pages for running process               |
|  4   | Dump PIDs of processes in the Multi-Level Feedback Queue (MLFQ) |
|  5   | Dump all PCBs                                                   |
|  6   | Dump PIDs of processes with open spool files.                   |

## Example Program
For an example, see this [program](factorial_program.txt) which computes `b = a! % |d|`
For more details on the program, see its [assembly](factorial_program.asm).


## Run
The computer expects to connect to the printer server, so ensure to run it as shown below.

### Run printer
This process will generate an output file named `printer.out` containing the results of all executed programs.
Note that each program's output is spooled.
  ````
  ./exec_printer.sh
  ````

### Run Computer
Below will execute a simulated computer that runs on MonoOS.
Ensure to have already completed [Run Printer](#run-printer).

  ````
  ./exec_computer.sh <insert computerID>
  ````

## File Structure
- `config.sys` contains the parameters needed for initializing the OS. 
  See [computer.cpp](computer.cpp) and [printer.cpp](printer.cpp) for more info on each parameter.
  Currently, `config.sys` is set to default values that will allow most programs to execute.
- The idle program is in `prog-idle.txt`. This ensures the CPU maintains operation even when no user processes are available.
  **Note that if you change this program, your new program must be an infinite loop.** 


````
    .
    └── MonoOS/
        ├── computer.cpp
        ├── load.cpp
        ├── print.cpp
        ├── printer.cpp
        ├── scheduler.cpp
        ├── shell.cpp
        ├── config.txt
        ├── computer/
        │   ├── computer.h
        │   └── print.h
        ├── connector/
        │   ├── client_connector.cpp
        │   ├── client_connector.h
        │   ├── server_connector.cpp
        │   └── server_connector.h
        ├── cpu/
        │   ├── cpu.cpp
        │   └── cpu.h
        ├── device_manager/
        │   └── printer/
        │       ├── printer.h
        │       ├── communicator/
        │       │   ├── communicator.cpp
        │       │   └── communicator.h
        │       └── printer_manager/
        │           ├── printer_manager.cpp
        │           └── printer_manager.h
        ├── memory/
        │   ├── loader/
        │   │   └── load.h
        │   ├── resident_set/
        │   │   ├── resident_set.cpp
        │   │   └── resident_set.h
        │   ├── memory.cpp
        │   └── memory.h
        ├── printer_utility/
        │   ├── printer_utility.cpp
        │   └── printer_utility.h
        ├── scheduler/
        │   ├── queue.cpp
        │   ├── queue.h
        │   └── scheduler.h
        ├── shell/
        │   └── shell.h
        └── utility/
            ├── computer_utility/
            │   ├── computer_utility.cpp
            │   └── computer_utility.h
            ├── file_utility/
            │   ├── file_utility.cpp
            │   └── file_utility.h
            ├── utility.cpp
            └── utility.h
    
````

## Architecture

### TODO


## MLFQ Scheduling
The scheduler uses a Multi-Level Feedback Queue (MLFQ) set to four levels
as defined in `rq_levels` found in `scheduler.cpp`.
Feel free to change this value to a reasonable (>1) positive integer.

The MLFQ simulates I/O delay with a configurable counter named `mem_delay` that is preset to `TQ` in `computer.cpp`.
Operationally, for an I/O instruction, the delay is simulated as follows:

- The CPU waits on executing the I/O instruction by instead incrementing a counter `current_mem_instr_time`.
- After each increment, PC is not advanced but instead the CPU keeps it at the same I/O instruction.
- Also after the increment, the scheduler switches the process out and dispatches another one from the ready queue.
- If `current_mem_instr_time` equals `mem_delay`, the CPU executes the delayed I/O instruction
  and advances PC to the next instruction.

Note that the scheduler stores `current_mem_instr_time` for the running process in `mem_instr_delay` within its PCB.
This procedure maintains the correctness of the delay simulation even when the process is switched out.
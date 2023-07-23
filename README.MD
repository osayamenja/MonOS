# MonoOS
A monolithic OS with MLFQ scheduling, on-demand paging, inter-process synchronization and communication and 
a device manager supporting network devices. 

On device management, MonoOS currently supports connections to a simulated printer. 

MonoOS also features a simulated CPU which executes user programs written as specified in x.

## Run
You have to execute the printer device first since the computer expects to connect to it.

### Run printer
**You must run this command, if you want to use the simulated printer.**
  ````
  ./exec_printer.sh
  ````


### Run Computer
Below will execute a simulated computer that runs on MonoOS.
Including `true` as an argument, means you have already completed [Run Printer](#run-printer).
If not, expect an error.

  ````
  ./exec_computer.sh <insert computerID> <insert 'true' to use simulated printer>
  ````

## File Structure
- 
- The idle program should be in `prog-idle.txt`.
  This is needed to ensure the CPU maintains operation even when no user processes are available. 


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
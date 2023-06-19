# MonoOS
A monolithic OS with MLFQ scheduling, memory manager supporting on-demand paging and 
a device manager supporting network devices. 

MonoOS also features a simulated CPU which executes user programs written as specified in  

### Note
Given that this is still an in-progress project, documentation will be slightly less than exhaustive.

However, we aim to complete this README as soon as changes are complete for the OS.


## Run
You have to execute the printer device first since the computer expects to connect to it.

### Run printer
  ````
  ./exec_printer.sh
  ````


### Run Computer
You can run the computer next
  ````
  ./exec_computer.sh
  ````


## File Structure
- See the below diagram to navigate the directory.
- The root directory is `MonoOS`.
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
        ├── makefile
        ├── computer/
        │   ├── computer.h
        │   └── print.h
        ├── computer_utility/
        │   ├── computer_utility.cpp
        │   └── computer_utility.h
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
        │       ├── printer_manager/
        │       │   ├── printer_manager.cpp
        │       │   └── printer_manager.h
        │       └── printer_utility/
        │           ├── printer_utility.cpp
        │           └── printer_utility.h
        │   
        ├── memory/
        │   ├── load.h
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
            ├── utility.cpp
            └── utility.h
    
````

## Architecture

### COMING SOON...


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
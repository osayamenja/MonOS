# MonoOS
A monolithic OS.

## Run
You have to build the `printer` and computer `executables` separately as shown below:

### Run printer
  ````
  make printer
  ````

  ````
  ./printer.exe
  ````

### Run Computer
  ````
  make computer
  ````

  ````
  ./computer.exe <cid>
  ````


## File Structure
- See the below diagram to navigate the directory.
- The root directory is `Aimuyo_OS_Project`.
- The idle program should be in `prog-idle.txt`.

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

## Documentation
Below are descriptions for the important `.cpp` files that comprise the program.

- ### scheduler.cpp
  implements all the functions defined in the handout, with a bonus of using the MLFQ algorithm. 
  See `Note` above for more details on its operation.
     
- ### print.cpp
  implements all the functions defined in the handout. 
  It receives a print_ACK after each interaction with `printer.cpp`.

- ### printer.cpp
  implements all the functions defined in the handout as a forked process. 
  It sends n print_ACK after each interaction with `print.cpp`.

- ### shell.cpp
  implements all the functions defined in the handout as a separate thread.

- ### cpu.cpp
  defines `cpu_operation()`, `cpu_fetch_instruction()`, `cpu_execute_instruction()`
  for cpu operations, fetching, and executing instructions, respectively.
  Also defines `cpu_mem_address(int m_addr)` for computing a true memory address, given `m_addr`
  and `cpu_dump_registers()` for dumping registers.

- ### memory.cpp
  defines `mem_init(int M)`, `mem_read()`, `mem_write()` and `mem_dump_memory()` for initializing, 
  reading from, writing to, and dumping the contents of memory, respectively.

- ### load.cpp
  defines `load_prog(char *fname, int base)` for loading instructions from file `fname` to
  memory address starting at `base`. Also, defines `load_finish(std::FILE *f)` which closes the opened program file.

- ### computer.cpp
  defines `main()` for starting the Operating System. Also defines `boot_system()` 
  for retrieving system configurations from `config.sys`. 

## Note
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
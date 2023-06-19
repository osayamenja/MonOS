cmake_minimum_required(VERSION 3.25)
project(MonoOS)

set(CMAKE_CXX_STANDARD 17)

add_library(utilities OBJECT
        utility/utility.cpp
        utility/utility.h)

add_executable(computer
        cpu/cpu.cpp
        computer.cpp
        memory/memory.cpp
        load.cpp shell.cpp
        computer/computer.h
        cpu/cpu.h
        memory/memory.h
        shell/shell.h
        scheduler.cpp
        scheduler/scheduler.h
        scheduler/queue.cpp
        scheduler/queue.h
        computer/print.h
        print.cpp
        memory/loader/load.h
        connector/client_connector.cpp
        connector/client_connector.h
        utility/computer_utility/computer_utility.cpp
        utility/computer_utility/computer_utility.h
        load.cpp memory/resident_set/resident_set.cpp
        memory/resident_set/resident_set.h
        memory/resident_set/resident_set.h)
target_link_libraries(computer utilities)

add_executable(printer
        printer.cpp
        device_manager/printer/printer.h
        device_manager/printer/printer_manager/printer_manager.cpp
        device_manager/printer/printer_manager/printer_manager.h
        device_manager/printer/printer_manager/printer_manager.h
        device_manager/printer/communicator/communicator.cpp
        device_manager/printer/communicator/communicator.h
        connector/server_connector.cpp
        connector/server_connector.h
        utility/file_utility/file_utility.cpp
        utility/file_utility/file_utility.h
        device_manager/printer/communicator/communicator.cpp)
target_link_libraries(printer utilities)

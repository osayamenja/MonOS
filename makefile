# Author:  Osayamen Aimuyo
# Created: 1/29/2023.
# Last Modified: 4/2/2023.

CC = g++

CFLAGS = -g -Wall -std=c++11 -pthread -lrt

computer: computer.o client_connector.o computer_utility.o cpu.o load.o memory.o print.o queue.o utility.o scheduler.o shell.o
	$(CC) $(CFLAGS) -o computer.exe computer.o client_connector.o computer_utility.o cpu.o load.o memory.o print.o queue.o utility.o scheduler.o shell.o

computer.o: computer.cpp computer/computer.h computer/print.h memory/memory.h scheduler/scheduler.h shell/shell.h utility/utility.h
	$(CC) $(CFLAGS) -c computer.cpp

printer: printer.o communicator.o printer_manager.o printer_utility.o utility.o server_connector.o
	$(CC) $(CFLAGS) -o printer.exe printer.o communicator.o printer_manager.o printer_utility.o utility.o server_connector.o

printer.o: printer.cpp computer/print.h printer/printer.h printer/communicator/communicator.h printer/printer_manager/printer_manager.h printer_utility/printer_utility.h utility/utility.h
	$(CC) $(CFLAGS) -c printer.cpp

client_connector.o: connector/client_connector.cpp connector/client_connector.h
	$(CC) $(CFLAGS) -c connector/client_connector.cpp

communicator.o: printer/communicator/communicator.cpp printer/printer.h printer/communicator/communicator.h printer/printer_manager/printer_manager.h printer_utility/printer_utility.h utility/utility.h
	$(CC) $(CFLAGS) -c printer/communicator/communicator.cpp

computer_utility.o: computer_utility/computer_utility.cpp computer_utility/computer_utility.h utility/utility.h
	$(CC) $(CFLAGS) -c computer_utility/computer_utility.cpp

cpu.o: cpu/cpu.cpp computer/computer.h computer/print.h cpu/cpu.h memory/memory.h scheduler/scheduler.h shell/shell.h utility/utility.h
	$(CC) $(CFLAGS) -c cpu/cpu.cpp

load.o: load.cpp computer_utility/computer_utility.h memory/load.h memory/memory.h utility/utility.h
	$(CC) $(CFLAGS) -c load.cpp

memory.o: memory/memory.cpp computer/computer.h memory/memory.h scheduler/scheduler.h utility/utility.h
	$(CC) $(CFLAGS) -c memory/memory.cpp

print.o: print.cpp computer/print.h computer/computer.h connector/client_connector.h utility/utility.h
	$(CC) $(CFLAGS) -c print.cpp

printer_manager.o: printer/printer_manager/printer_manager.cpp connector/server_connector.h printer/communicator/communicator.h printer/printer.h printer/printer_manager/printer_manager.h utility/utility.h
	$(CC) $(CFLAGS) -c printer/printer_manager/printer_manager.cpp

printer_utility.o: printer_utility/printer_utility.cpp printer_utility/printer_utility.h printer/printer.h
	$(CC) $(CFLAGS) -c printer_utility/printer_utility.cpp

queue.o: scheduler/queue.cpp computer/computer.h utility/utility.h
	$(CC) $(CFLAGS) -c scheduler/queue.cpp

scheduler.o: scheduler.cpp computer/print.h computer_utility/computer_utility.h scheduler/queue.h scheduler/scheduler.h utility/utility.h
	$(CC) $(CFLAGS) -c scheduler.cpp

server_connector.o: connector/server_connector.cpp connector/server_connector.h
	$(CC) $(CFLAGS) -c connector/server_connector.cpp

shell.o: shell.cpp computer/computer.h computer/print.h memory/memory.h scheduler/scheduler.h shell/shell.h utility/utility.h
	$(CC) $(CFLAGS) -c shell.cpp

utility.o: utility/utility.cpp printer/printer.h utility/utility.h
	$(CC) $(CFLAGS) -c utility/utility.cpp

clean_computer:
	rm -rf *.o computer.exe

clean_computer:
	rm -rf *.o printer.exe

clean:
	rm -rf *.o *.exe
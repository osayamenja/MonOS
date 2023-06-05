//
// Created by Osayamen on 4/1/2023.
//

#include <arpa/inet.h>
#include <semaphore.h>
#include <deque>
#include <mutex>
#include <thread>
#include "printer_manager.h"
#include "../../../connector/server_connector.h"
#include "../printer.h"
#include "../communicator/communicator.h"

int printer_server;
struct sockaddr_in address{};
int addr_len = sizeof(address);

sem_t full;
sem_t empty;
sem_t manager_init;
sem_t print_comm_init;
sem_t cq_mtx;
sem_t sync_pc;

std::deque<int> connection_queue;
void printer_manager_init(int nc, int cqs, int mqs){
    sem_init(&manager_init, 0, 0);
    sem_init(&print_comm_init, 0, 0);

    sem_init(&full, 0, cqs);
    sem_init(&empty, 0, 0);
    sem_init(&cq_mtx, 0, 1);
    sem_init(&sync_pc, 0, 0);

    connection_queue = std::deque<int>();

    communicators_init(nc, mqs); // communicators -> start_printer() -> manager
    start_printer();

    sem_wait(&manager_init); // wait for start_printer
    sem_wait(&manager_init); //  wait for communicators_init
    sem_destroy(&manager_init);


    printer_server = server_create();
    server_bind(printer_server, port, address, ip_addr);
    server_listen(printer_server, 5);
}

void printer_manager(int nc, int cqs, int mqs){
    atomically_print_to_stdout("Printer Manager Starting...");
    printer_manager_init(nc, cqs, mqs);
    atomically_print_to_stdout("Printer Manager is ready to accept connections!");
    while(!printer_terminate_flag){
        int sock = server_accept(printer_server, address, addr_len);
        connection_queue_insert(sock);
    }
}

void start_printer(){
    std::thread printer_worker(printer_main);
    printer_worker.detach();
}

void connection_queue_insert(int conn){
    sem_wait(&full);
    sem_wait(&cq_mtx);
    connection_queue.push_back(conn);
    sem_post(&cq_mtx);
    sem_post(&empty);
}

int connection_queue_consume(){
    sem_wait(&empty);
    sem_wait(&cq_mtx);
    int conn = connection_queue.front();
    connection_queue.pop_front();
    sem_post(&full);
    sem_post(&cq_mtx);
    return conn;
}


//
// Created by Osayamen on 4/1/2023.
//

#include <vector>
#include <deque>
#include <semaphore.h>
#include <mutex>
#include <thread>
#include <unistd.h>

#include "communicator.h"
#include "../printer.h"
#include "../printer_manager/printer_manager.h"
#include "../../../utility/file_utility/file_utility.h"

std::vector<sem_t> mq_sync_semaphores;
std::vector<std::mutex> mq_mutexes;
std::vector<std::deque<std::string>> messageQueues;

int mq_size;
void communicator_init(int id){
    std::thread comm (communicator, id);
    comm.detach();
}

void communicators_init(int nc, int mqs){
    mq_size = mqs;
    messageQueues = std::vector<std::deque<std::string>>(nc);
    mq_sync_semaphores = std::vector<sem_t>(nc);
    mq_mutexes = std::vector<std::mutex>(nc);

    for(int i = 0; i < nc; ++i){
        communicator_init(i);
    }

    sem_post(&print_comm_init);
    sem_post(&manager_init);
    sem_destroy(&print_comm_init);
}

void communicator(int id){
    int myID = id;
    messageQueues.at(id) = std::deque<std::string>();
    sem_init(&mq_sync_semaphores.at(id), 0, mq_size);

    while(!printer_terminate_flag){
        char message [128];
        int sock = connection_queue_consume();
        sendInt(sock, COMMUNICATOR_ACK, 0); // send an ACK

        readAny(sock, message, sizeof(message), 0); // receive computer ID.
        int comp_id = conv_to_int(message);
        sendInt(sock, myID, 0); // send my id;

        std::string notification = std::string ("Communicator ")
                .append(std::to_string(id))
                .append(" won a connection!");

        atomically_print_to_stdout(notification);

        auto still_open = readAny(sock, message, sizeof (message), 0);

        while(still_open > 0 && !printer_terminate_flag){
            message_queues_insert(std::string(message), id);
            sem_post(&sync_pc);
            sendInt(sock, COMMUNICATOR_ACK, 0); // send an ACK.

            // still_open == 0 when the socket closes, which occurs after terminate concludes.
            still_open = readAny(sock, message, sizeof(message), 0);
        }
        close(sock);
        atomically_print_to_stdout(std::string("Socket connection to ")
        .append(std::to_string(comp_id).append(" is terminated!")));
    }
}

void message_queues_consume(){
    int i = 0;
    for(auto &messageQueue : messageQueues){
        mq_mutexes.at(i).lock();
        while(!messageQueue.empty()){
            printerOps_queue_insert(messageQueue.front());
            messageQueue.pop_front();
            sem_post(&mq_sync_semaphores.at(i));
        }
        mq_mutexes.at(i).unlock();
        i++;
    }
}

void message_queues_insert(const std::string& m, int id){
    sem_wait(&mq_sync_semaphores.at(id));
    mq_mutexes.at(id).lock();
    messageQueues.at(id).push_back(m);
    mq_mutexes.at(id).unlock();
}

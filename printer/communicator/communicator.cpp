//
// Created by Osayamen on 4/1/2023.
//

#include <vector>
#include <deque>
#include <semaphore.h>
#include <mutex>
#include <thread>

#include "communicator.h"
#include "../printer.h"
#include "../printer_manager/printer_manager.h"
#include "../../printer_utility/printer_utility.h"

#define ERROR_STATUS (-1)

std::vector<sem_t> mq_sync_semaphores;
std::vector<std::mutex> mq_mutexes;
std::vector<sem_t> terminate_semaphores;
std::vector<std::deque<std::vector<int>>> messageQueues;

int mq_size;
int communicator_ACK = 1;
void communicator_init(int id){
    std::thread comm (communicator, id);
    comm.detach();
}

void communicators_init(int nc, int mqs){
    mq_size = mqs;
    messageQueues = std::vector<std::deque<std::vector<int>>>(nc);
    mq_sync_semaphores = std::vector<sem_t>(nc);
    terminate_semaphores = std::vector<sem_t>(nc);
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
    messageQueues.at(id) = std::deque<std::vector<int>>();
    sem_init(&mq_sync_semaphores.at(id), 0, mq_size);
    sem_init(&terminate_semaphores.at(id), 0, 0);

    int comp_ACK;
    int printer_op, comp_id, pid, buf;
    while(!printer_terminate_flag){
        int sock = connection_queue_consume();
        sendAll(sock, communicator_ACK, 0); // send my print_ACK;

        readAll(sock, &comp_ACK); // read their print_ACK
        if(comp_ACK != communicator_ACK){
            perror("Failed to hear back from connection!");
            exit(ERROR_STATUS);
        }

        sendAll(sock, myID, 0); // send my id;

        std::string notification = std::string ("Communicator ")
                .append(std::to_string(id))
                .append(" won a connection!");

        atomically_print_to_stdout(notification);


        auto still_open = readAll(sock, &printer_op);

        while(still_open > 0 && !printer_terminate_flag){
            sendAll(sock, communicator_ACK, 0);

            auto op = getPrinterOp(printer_op);

            readAll(sock, &comp_id);
            sendAll(sock, communicator_ACK, 0);

            switch(op){
                case INIT_SPOOL:
                case END_SPOOL:
                    readAll(sock, &pid);
                    sendAll(sock, communicator_ACK, 0);
                    break;
                case PRINT:
                    readAll(sock, &pid);
                    sendAll(sock, communicator_ACK, 0);

                    readAll(sock, &buf);
                    sendAll(sock, communicator_ACK, 0);
                    break;
                default:
                    break;
            }

            std::vector<int> msg = {printer_op, comp_id, pid, buf};

            message_queues_insert(msg, id);
            sendAll(sock, communicator_ACK, 0);

            sem_post(&sync_pc);

            if(op == TERMINATE){ // wait for signal from printer before exiting
                sem_wait(&(terminate_semaphores.at(id)));

                sendAll(sock, communicator_ACK, 0);
            }

            /*flush the integer buffers*/
            printer_op = 0;
            comp_id = 0;
            pid = 0;
            buf = 0;
            /*************************/

            // still_open == 0 when the socket closes, which occurs after terminate concludes.
            still_open = readAll(sock, &printer_op);
        }
        atomically_print_to_stdout(std::string("Socket connection to ")
        .append(std::to_string(comp_id).append(" is terminated!")));
    }
}

void message_queues_consume(){
    int i = 0;
    for(auto &messageQueue : messageQueues){
        mq_mutexes.at(i).lock();
        while(!messageQueue.empty()){
            printerOps_queue_insert(messageQueue.front(), i);
            messageQueue.pop_front();
            sem_post(&mq_sync_semaphores.at(i));
        }
        mq_mutexes.at(i).unlock();
        i++;
    }
}

void message_queues_insert(const std::vector<int>& m, int id){
    sem_wait(&mq_sync_semaphores.at(id));
    mq_mutexes.at(id).lock();
    messageQueues.at(id).push_back(m);
    mq_mutexes.at(id).unlock();
}

void signal_termination(int id){
    sem_post(&(terminate_semaphores.at(id)));
}

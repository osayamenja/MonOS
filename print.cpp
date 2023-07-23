//
// Created by Osayamen on 2/25/2023.
//

#include <unistd.h>

#include "computer/print.h"
#include "utility/utility.h"
#include "connector/client_connector.h"
#include "computer/computer.h"

int comp_sock;
int commID;
int cid;

void print_act(const std::vector<int>& action, const std::string& name, const std::string& success_msg){
    char ack[16];
    std::string msg = build_msg(action);
    sendAny(comp_sock, msg.data(), msg.size(), 0);
    readAndVerifyInt(comp_sock,
                     ack,
                     get_size(COMMUNICATOR_ACK),
                     0,
                     COMMUNICATOR_ACK,
                     name);
    if(!success_msg.empty()) atomically_print_to_stdout(success_msg);
}

void print_init(){
    // set up client socket connection
    atomically_print_to_stdout("Connecting to Printer Server...");
    cid = CID;
    comp_sock = client_connect(port, ip_addr);
    char buffer[16];

    readAndVerifyInt(comp_sock,
            buffer,
            get_size(COMMUNICATOR_ACK),
            0,
            COMMUNICATOR_ACK,
            "printer_init::COMM_ACK");

    sendInt(comp_sock, cid, 0);

    readAny(comp_sock, buffer, sizeof(buffer), 0);
    commID = conv_to_int(buffer);
    atomically_print_to_stdout("Connected to Communicator " + std::to_string(commID) + "!");
}

void stop_print(){
    close(comp_sock);
}


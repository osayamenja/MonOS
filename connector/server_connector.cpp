//
// Created by Osayamen on 4/1/2023.
//

#include <arpa/inet.h>
#include <sys/socket.h>
#include "server_connector.h"

#define ERROR_CODE (-1)

int server_create(){
    int server;
    int socket_opt = 1;
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Server failed to Create");
        exit(ERROR_CODE);
    }

    if (setsockopt(server,
                   SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &socket_opt,sizeof(int))) {
        perror("Server failed to Create");
        exit(ERROR_CODE);
    }
    return server;
}

void server_bind(int server, int port, sockaddr_in address, const std::string& ip_address){
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address.c_str(), &address.sin_addr);
    address.sin_port = htons(port);

    if (bind(server, (struct sockaddr*)&address,sizeof(address)) < 0) {
        perror("Server failed to bind");
        exit(ERROR_CODE);
    }
}

void server_listen(int server, int backlog){
    if (listen(server, backlog) < 0) {
        perror("Server failed to listen");
        exit(EXIT_FAILURE);
    }
}

int server_accept(int server, sockaddr_in address, int address_len){
    int connected_socket;
    if ((connected_socket = accept(server, (struct sockaddr*)&address, (socklen_t*)&address_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return connected_socket;
}

void server_destroy(int server){
    shutdown(server, SHUT_RDWR);
}

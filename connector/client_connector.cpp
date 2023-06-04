//
// Created by Osayamen on 4/1/2023.
//

#include <arpa/inet.h>
#include <string>
#include <tuple>
#include "client_connector.h"

std::tuple<int, int> client_connect(int port, const std::string& ip_address){
    int client_socket;
    int client;
    struct sockaddr_in serv_addr{};
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed to create");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("Invalid IP Address");
        exit(-1);
    }

    if ((client = connect(client_socket, (struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0) {
        perror("Client connection failed");
        exit(-1);
    }
    return std::make_tuple(client_socket, client);
}



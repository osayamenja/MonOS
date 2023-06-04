//
// Created by Osayamen on 4/1/2023.
//

#ifndef AIMUYO_OS_PROJECT_2_SERVER_CONNECTOR_H
#define AIMUYO_OS_PROJECT_2_SERVER_CONNECTOR_H

#include <string>

int server_create();
void server_bind(int server, int port, sockaddr_in address, const std::string& ip_address);
void server_listen(int server, int backlog);
int server_accept(int server, sockaddr_in address, int address_len);
void server_destroy(int server);

#endif //AIMUYO_OS_PROJECT_2_SERVER_CONNECTOR_H

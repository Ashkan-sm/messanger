//
// Created by dev on 1/15/25.
//
#ifndef CHATROOM_CONNECTION_SERVER_H
#define CHATROOM_CONNECTION_SERVER_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "File_Handle.h"

class Connection_Server {
    int MAX_CLIENTS=100;
    int HEADER_SIZE=10;




    struct sockaddr_in srv;
    struct in_addr localInterface;

    bool is_udp_;

public:

    explicit Connection_Server();
    ~Connection_Server();
    explicit Connection_Server(uint16_t PORT,in_addr_t IP=INADDR_ANY,bool is_udp=false);

    fd_set fr, fw, fe;
    uint16_t PORT_;
    in_addr_t IP_;
    int socket_;

    std::string getEth0IPAddress();
    void refresh_socket();
    void ready_socket(int socket_to_set);
    int select_socket(int to_max_num);

    void send_on_multicast(std::string my_ip);
    int accept_connection();
    void send_msg_to_sock(int sock,const char* msg);
    int read_msg_from_sock(int sock,char *buffer);
    void send_file_to_sock(int socket,std::string filename);
    void read_file_from_sock(int socket,std::string filename);
};


#endif //CHATROOM_CONNECTION_SERVER_H

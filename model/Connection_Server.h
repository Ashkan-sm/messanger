//
// Created by dev on 1/15/25.
//
#ifndef CHATROOM_CONNECTION_SERVER_H
#define CHATROOM_CONNECTION_SERVER_H

#include <string>
#include <ifaddrs.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "File_Handle.h"
#include <zmq.hpp>
#include <chrono>
#include <thread>

class Connection_Server {
    int MAX_CLIENTS=100;
    int HEADER_SIZE=10;




    struct sockaddr_in srv;
    struct in_addr localInterface;

    bool is_udp_;
    zmq::context_t context{1};
public:

    explicit Connection_Server();
    ~Connection_Server();
    explicit Connection_Server(uint16_t PORT,in_addr_t IP=INADDR_ANY,bool is_udp=false);

    fd_set fr, fw, fe;
    uint16_t PORT_;
    in_addr_t IP_;
    zmq::socket_t *socket;

    std::string getEth0IPAddress();
    std::pair<std::string,std::string> recv();
    void send_on_multicast(std::string my_ip);
    void send_msg_to_sock(std::string id,std::string msg);
    int read_msg_from_sock(int sock,char *buffer);
    void send_file_to_sock(int socket,std::string filename);
    void read_file_from_sock(int socket,std::string filename);
};


#endif //CHATROOM_CONNECTION_SERVER_H

//
// Created by dev on 1/15/25.
//

#ifndef CHATROOM_CONNECTION_CLIENT_H
#define CHATROOM_CONNECTION_CLIENT_H

#include <thread>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QQmlContext>
#include<cstdio>
#include<iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fstream>
#include <ifaddrs.h>
#include <zmq.hpp>
#include "client.h"

class Client;

class Connection_Client {
private:
    struct sockaddr_in localSock;
    struct ip_mreq group;
    int port_;
    in_addr_t ip_;
    bool is_udp_;
    int nClientSocket;
    int HEADER_SIZE=10;
    std::vector<std::string> logs;
    std::ifstream::pos_type filesize(const char* filename);
    zmq::context_t context{1};
    zmq::socket_t *socket;
public:
    int readmsg(char* out);
    explicit Connection_Client();
    explicit Connection_Client(int PORT,in_addr_t IP=INADDR_ANY,bool is_udp= false);
    int read_sock(char *databuf,int size);\
    std::string recv();
    void writemssg(const char *msg);
    void sendfile(std::string filename,QString to);
    std::string revcfile(std::string filename);
    void connection_manager(Client* client_pointer);

};


#endif //CHATROOM_CONNECTION_CLIENT_H

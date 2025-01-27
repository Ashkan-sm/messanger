//
// Created by dev on 1/15/25.
//

#ifndef CHATROOM_CONNECTION_CLIENT_H
#define CHATROOM_CONNECTION_CLIENT_H

#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QQmlContext>
#include<cstdio>
#include<iostream>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fstream>
#include <ifaddrs.h>

#include "client.h"

class Client;

class Connection_Client {
private:
    struct sockaddr_in localSock;
    struct ip_mreq group;
    int port_;
    in_addr_t ip_;
    bool is_udp_;

    int HEADER_SIZE=10;
    std::vector<std::string> logs;

public:
    int readmsg(int socket,char* out);
    Connection_Client();
    Connection_Client(int PORT,in_addr_t IP=INADDR_ANY,bool is_udp= false);
    int read_sock(char *databuf,int size);
    int nClientSocket;
    void connection_manager(Client* client_pointer);

};


#endif //CHATROOM_CONNECTION_CLIENT_H

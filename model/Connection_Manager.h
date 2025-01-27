//
// Created by dev on 1/20/25.
//

#ifndef CHATROOM_CONNECTION_MANAGER_H
#define CHATROOM_CONNECTION_MANAGER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/select.h>
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <thread>
#include <cstdlib>
#include <vector>
#include <memory>

#include "Server_Obj.h"
#include "Connection_Server.h"
#include "User.h"

class Server_Obj;
class Connection_Manager {
    struct in_addr localInterface;
    int nMaxFd;
    int nRet=0;


    Connection_Server cnc_handle;
    Connection_Server broadcast_handle;
    Server_Obj* server;

    static void pop_user_by_socket(std::vector<User> &list,int sock);
    static int find_user_id(std::vector<User> &list,std::string id);

public:
    Connection_Manager(int port_tcp,int port_udp,std::string udp_group_ip);
    void start_server();
    int MAX_CLIENTS=100;
    std::vector<User> ArrClient;
    void send_message_(std::string client_id,std::string message);
    void send_file_(std::string client_id,std::string message);
    void read_file_(std::string client_id,std::string message);
    void logout(std::string id);
    void join_exec();

    std::shared_ptr<std::thread> exec_thread_;

};


#endif //CHATROOM_CONNECTION_MANAGER_H

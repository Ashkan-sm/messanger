//
// Created by dev on 1/20/25.
//

#ifndef CHATROOM_CONNECTION_MANAGER_H
#define CHATROOM_CONNECTION_MANAGER_H

#include <iostream>
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
#include <map>
#include "Server_Obj.h"
#include "Connection_Server.h"
#include "User.h"

class Server_Obj;
class Connection_Manager {
private:
    int nMaxFd;
    int nRet=0;
    Connection_Server *cnc_handle;
    Connection_Server *broadcast_handle;
    std::shared_ptr<std::thread> exec_thread_;
    Server_Obj* server;
    Connection_Manager();
    static void pop_user_by_socket(std::vector<User> &list,int sock);

    static Connection_Manager* instance;
    int MAX_CLIENTS=100;
    std::map<std::string,User> ArrClient;

public:
    static Connection_Manager& get_instance();
    void set_up(int port_tcp,int port_udp,std::string udp_group_ip);
    void start_server();
    void join_exec();

    void send_file_(std::string client_id,std::string message);
    void read_file_(std::string client_id,std::string message);
    void logout(std::string id);
    void send_message_(std::string client_id,std::string message);
    std::map<std::string,User>& getArrClient() ;

};


#endif //CHATROOM_CONNECTION_MANAGER_H

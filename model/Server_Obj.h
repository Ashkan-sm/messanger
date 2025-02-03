//
// Created by dev on 1/15/25.
//

#ifndef CHATROOM_SERVER_OBJ_H
#define CHATROOM_SERVER_OBJ_H

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

#include "Connection_Manager.h"
#include "User.h"

class Connection_Manager;

class Server_Obj {
    int find_user_id(std::vector<User> &list,std::string id);
public:
    explicit Server_Obj();
    void run(Connection_Manager* conection_manager, std::string id,std::vector<std::string> log_arr);

};


#endif //CHATROOM_SERVER_OBJ_H

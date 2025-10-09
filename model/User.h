//
// Created by dev on 1/15/25.
//
#ifndef CHATROOM_USER_H
#define CHATROOM_USER_H

#include <string>
#include <vector>
class User {

public:
    int get_socket() const;
    explicit User();
    explicit User(int socket,std::string id="");
    ~User();
    int socket_;
    std::string name_;
    bool authenticated=false;
private:

};

#endif //CHATROOM_USER_H

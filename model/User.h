//
// Created by dev on 1/15/25.
//
#ifndef CHATROOM_USER_H
#define CHATROOM_USER_H

#include <string>
#include <vector>
class User {
    explicit User();
public:
    std::vector<std::string> logs;
    int get_socket() const;
    explicit User(int socket,std::string id="");
    ~User();
    int socket_;
    std::string id_;
private:

};

#endif //CHATROOM_USER_H

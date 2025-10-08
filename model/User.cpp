//
// Created by dev on 1/15/25.
//

#include "User.h"


User::User(int socket, std::string name) {
    socket_=socket;
    name_=name;
}

User::~User() {

}

int User::get_socket() const {
    return socket_;
}

User::User() {

}

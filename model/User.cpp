//
// Created by dev on 1/15/25.
//

#include "User.h"


User::User(int socket, std::string id) {
    socket_=socket;
    id_=id;
}

User::~User() {

}

int User::get_socket() const {
    return socket_;
}

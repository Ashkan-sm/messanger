//
// Created by dev on 1/15/25.
//

#include "Connection_Server.h"
#include <istream>
Connection_Server::Connection_Server(uint16_t PORT, in_addr_t IP,bool is_udp) {
    PORT_=PORT;
    IP_=IP;
    is_udp_=is_udp;

    if(!is_udp) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &IP_, ipStr, INET_ADDRSTRLEN);
        socket = new zmq::socket_t(context, zmq::socket_type::router);
        socket->bind("tcp://"+ (std::string)ipStr +":" + std::to_string(PORT));
    }
    else {

        socket = new zmq::socket_t(context, zmq::socket_type::pub);
        socket->bind("tcp://*:12345");

    }


}

Connection_Server::Connection_Server() {

}

Connection_Server::~Connection_Server() {

}

std::string Connection_Server::getEth0IPAddress(){
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    // Get the list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        std::cerr<<"getifaddrs"<<std::endl;
        return "";
    }

    // Loop through interfaces to find eth0
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) {
            continue;
        }

        // Check if this is eth0 and if it's an IPv4 address
        //std::string(ifa->ifa_name) == "enp4s0" &&
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;

            // Convert the address to a human-readable string
            if (inet_ntop(AF_INET, &addr->sin_addr, host, sizeof(host)) == nullptr) {
                perror("inet_ntop");
                std::cerr<<"inet_ntop"<<std::endl;
                freeifaddrs(ifaddr);
                return "";
            }

            freeifaddrs(ifaddr); // Free the memory allocated by getifaddrs
            return std::string(host);
        }
    }

    freeifaddrs(ifaddr); // Free the memory allocated by getifaddrs
    return ""; // eth0 not found
}


void Connection_Server::send_on_multicast(std::string my_ip) {
    std::string multiCastMsg = my_ip;
    std::cout<<"broadcasted msg: "<<my_ip<<std::endl;
    while (1) {
        socket->send(zmq::buffer(my_ip),zmq::send_flags::none);
        sleep(1);
    }
}


void Connection_Server::send_msg_to_sock(std::string id,std::string msg) {
    zmq::message_t id_reply(id.data(), id.size());
    zmq::message_t empty_reply(0);
    zmq::message_t reply(msg.c_str(), msg.size());

    socket->send(id_reply, zmq::send_flags::sndmore);
    socket->send(empty_reply, zmq::send_flags::sndmore);
    socket->send(reply, zmq::send_flags::none);
}

void Connection_Server::send_file_to_sock(int sock, std::string filename) {

//    File_Handle file("./recived_files/"+filename,File_Handle::Method::IN);
//
//    if (file.file_.is_open()){
//        send_msg_to_sock(sock,("/sending_file "+filename).c_str());
//        send_msg_to_sock(sock,"EOLOG");
//
//        long long int size=file.size_of_file("./recived_files/"+filename);
//        std::cout<<size<<std::endl;
//        send_msg_to_sock(sock, std::to_string(size).c_str());
//        char buffer[1024];
//        do {
//            file.file_.read(buffer,sizeof(buffer));
//            if (file.file_.gcount()){
//                const auto sent = send(sock,buffer,file.file_.gcount(),0);
//
//            }
//        } while (!file.file_.eof());
//
//    }
//
//    else{
//        std::cout<<"didnt find reqested file :"<<"./recived_files/"+filename<<std::endl;
//    }
//    file.file_.close();
//    return;
}

int Connection_Server::read_msg_from_sock(int sock, char *buffer) {

}

void Connection_Server::read_file_from_sock(int sock, std::string filename) {
//    std::cout<<"getting file: "<<std::endl;
//    File_Handle resivedfile("./recived_files/"+filename.substr(filename.rfind('/')+1),File_Handle::Method::OUT);
//
//    ssize_t bytes_received;
//
//    char buffer[1024];
//    char sizestr[1024];
//    read_msg_from_sock(sock,sizestr);
//    long long int size = atoi(sizestr);
//    std::cout << "sizeof file getinig: "<<size<<std::endl;
//    long long int recvCounter=0;
//    while(recvCounter+1024<size){
//        bytes_received = recv(sock, buffer, sizeof(buffer), 0);
//        resivedfile.file_.write(buffer,bytes_received);
//        recvCounter += bytes_received;
//    }
//    bytes_received = recv(sock, buffer, size-recvCounter, 0);
//    resivedfile.file_.write(buffer,bytes_received);
//    resivedfile.file_.close();
//    std::cout<<"savedfile"<<std::endl;
//    return;
}

std::pair<std::string,std::string> Connection_Server::recv() {
    zmq::message_t client_id;
    zmq::message_t empty;
    zmq::message_t request;

    socket->recv(client_id);
    socket->recv(empty);   // empty frame
    socket->recv(request);

    std::string id_str(static_cast<char*>(client_id.data()), client_id.size());
    std::string msg(static_cast<char*>(request.data()), request.size());
    return {id_str,msg};
}




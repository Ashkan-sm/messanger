//
// Created by dev on 1/15/25.
//

#include "Connection_Server.h"
#include <istream>
Connection_Server::Connection_Server(uint16_t PORT, in_addr_t IP,bool is_udp) {
    PORT_=PORT;
    IP_=IP;
    is_udp_=is_udp;

    if (is_udp_){
        socket_=socket(AF_INET, SOCK_DGRAM, 0);
        localInterface.s_addr = INADDR_ANY;

    } else{
        socket_=socket(AF_INET, SOCK_STREAM, 0);

    }
    if (socket_ < 0) {
        std::cerr << "Socket creation failed\n";
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT_);
    srv.sin_addr.s_addr = IP_;
    memset(&(srv.sin_zero), 0, 8);

    if (is_udp_){
        std::cout<<"my ip: "<<gethostid()<<std::endl;
        if(setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
        {
            perror("Setting local interface error");
            exit(1);
        }
        else
            printf("Setting the local interface...OK\n");
    }
    else {
        int noptval = 1;
        int noptlen = sizeof(noptval);
        if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (const char *) &noptval, noptlen)) {
            std::cerr << "setsockopt failed\n";
        }

        if (bind(socket_, (struct sockaddr *) &srv, sizeof(srv)) < 0) {
            std::cerr << "Failed to bind\n";
            close(socket_);
        }
        if (listen(socket_, MAX_CLIENTS) < 0) {
            std::cerr << "Failed to listen\n";
        }
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
        return "";
    }

    // Loop through interfaces to find eth0
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) {
            continue;
        }

        // Check if this is eth0 and if it's an IPv4 address
        if (std::string(ifa->ifa_name) == "enp4s0" && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;

            // Convert the address to a human-readable string
            if (inet_ntop(AF_INET, &addr->sin_addr, host, sizeof(host)) == nullptr) {
                perror("inet_ntop");
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

void Connection_Server::refresh_socket() {
    FD_ZERO(&fr);
    FD_ZERO(&fw);
    FD_ZERO(&fe);
}

void Connection_Server::ready_socket(int socket_to_set) {
    FD_SET(socket_to_set, &fr);
    FD_SET(socket_to_set, &fe);
}

int Connection_Server::select_socket(int to_max_num) {
    return select(to_max_num + 1, &fr, &fw, &fe, NULL);  // Blocking select call
}

void Connection_Server::send_on_multicast(std::string my_ip) {
    std::string multiCastMsg = my_ip;
    while (1) {
        if (sendto(socket_, multiCastMsg.c_str(), multiCastMsg.length(), 0, (struct sockaddr *) &srv,
                   sizeof(srv)) < 0) { perror("Sending datagram message error"); }
        sleep(1);
    }
}

int Connection_Server::accept_connection() {

    if (FD_ISSET(socket_, &fr)) {
        socklen_t nLen = sizeof(struct sockaddr);
        return accept(socket_, NULL, &nLen);
    }
    return 0;
}

void Connection_Server::send_msg_to_sock(int sock,const char *msg) {
    std::string msgstr(msg);
    std::string header(10,' ');
    std::string msglength=std::to_string(msgstr.length());
    header.replace(0,msglength.length(),msglength);
    std::string buffer(header+msgstr);
    send(sock,buffer.c_str(),buffer.length(),0);
}

void Connection_Server::send_file_to_sock(int sock, std::string filename) {

    File_Handle file("./recived_files/"+filename,'b');

    if (file.file_.is_open()){
        send_msg_to_sock(sock,("/sending_file "+filename).c_str());
        send_msg_to_sock(sock,"EOLOG");

        long long int size=file.size_of_file("./recived_files/"+filename);
        std::cout<<size<<std::endl;
        send_msg_to_sock(sock, std::to_string(size).c_str());
        char buffer[1024];
        do {
            file.file_.read(buffer,sizeof(buffer));
            if (file.file_.gcount()){
                const auto sent = send(sock,buffer,file.file_.gcount(),0);

            }
        } while (!file.file_.eof());

    }

    else{
        std::cout<<"didnt find reqested file :"<<"./recived_files/"+filename<<std::endl;
    }
    file.file_.close();
    return;
}

int Connection_Server::read_msg_from_sock(int sock, char *buffer) {
    memset(buffer, '\0', strlen(buffer));
    char header[HEADER_SIZE+1];
    recv(sock,header,HEADER_SIZE,0);
    header[HEADER_SIZE]='\0';
    //std::string headstr(header);
    int size=std::atoi(header);

    int o;
    o=recv(sock,buffer,size,0);
    buffer[o]='\0';
    return o;
}

void Connection_Server::read_file_from_sock(int sock, std::string filename) {
    std::cout<<"getting file: "<<std::endl;
    File_Handle resivedfile("./recived_files/"+filename.substr(filename.rfind('/')+1),'w');

    ssize_t bytes_received;

    char buffer[1024];
    char sizestr[1024];
    read_msg_from_sock(sock,sizestr);
    long long int size = atoi(sizestr);
    std::cout << "sizeof file getinig: "<<size<<std::endl;
    long long int recvCounter=0;
    while(recvCounter+1024<size){
        bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        resivedfile.file_.write(buffer,bytes_received);
        recvCounter += bytes_received;
    }
    bytes_received = recv(sock, buffer, size-recvCounter, 0);
    resivedfile.file_.write(buffer,bytes_received);
    resivedfile.file_.close();
    std::cout<<"savedfile"<<std::endl;
    return;
}




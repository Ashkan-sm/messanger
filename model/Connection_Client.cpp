//
// Created by dev on 1/15/25.
//

#include "Connection_Client.h"

Connection_Client::Connection_Client(int PORT,in_addr_t IP,bool is_udp) {
    port_=PORT;
    ip_=IP;
    is_udp_=is_udp;

    if (is_udp_){
        nClientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if(nClientSocket < 0)
        {
            perror("Opening datagram socket error");
            exit(1);
        }

        else
            printf("Opening datagram socket....OK.\n");

        memset((char *) &localSock, 0, sizeof(localSock));
        localSock.sin_family = AF_INET;
        localSock.sin_port = htons(port_);
        localSock.sin_addr.s_addr = INADDR_ANY;

        {
            int reuse = 1;
            if(setsockopt(nClientSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
            {
                perror("Setting SO_REUSEADDR error");
                close(nClientSocket);
                exit(1);
            }
            else
                printf("Setting SO_REUSEADDR...OK.\n");
        }

        if(bind(nClientSocket, (struct sockaddr*)&localSock, sizeof(localSock)))
        {
            perror("Binding datagram socket error");
            close(nClientSocket);
            exit(1);
        }
        else
            printf("Binding datagram socket...OK.\n");

        group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
        group.imr_interface.s_addr = INADDR_ANY;


        if(setsockopt(nClientSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
        {
            perror("Adding multicast group error");
            close(nClientSocket);
            exit(1);
        }
        else
            printf("Adding multicast group...OK.\n");


    }
    else{
        nClientSocket =  socket(AF_INET, SOCK_STREAM, 0);
        if (nClientSocket < 0) {
            std::cerr << "Socket creation failed\n";
            return;
        }


        localSock.sin_family = AF_INET;
        localSock.sin_port = htons(port_);
        localSock.sin_addr.s_addr= ip_;
        memset(&(localSock.sin_zero),0,8);

        if(auto code = ::connect(nClientSocket, (struct sockaddr*) & localSock, sizeof(localSock)) < 0){
            qInfo()<<"conection failed: "<<strerror(errno);
        }
    }






}

int Connection_Client::read_sock(char *databuf,int size) {
    return read(nClientSocket, databuf,size);
}
int Connection_Client::readmsg(int socket, char *out)
{
    memset(out, '\0', strlen(out));
    char header[HEADER_SIZE];
    recv(socket,header,HEADER_SIZE,0);
    header[HEADER_SIZE]='\0';
    //std::string headstr(header);

    int size=std::atoi(header);
    int o;
    o=recv(socket,out,size,0);
    out[o]='\0';
//    std::cout<<"got from server: "<<out<<std::endl;
    return o;
}

void Connection_Client::connection_manager(Client* client_pointer) {
    while(1){
        char buffer[4096]={0};
        qInfo()<<"reading";
        readmsg(nClientSocket,buffer);
        logs.push_back((buffer));
        std::cout<<"got : "<<buffer<<std::endl;
        if (strncmp(buffer,"EOLOG",strlen("EOLOG"))==0){
            client_pointer->client_run(logs);
            logs.clear();
        }

    }
}

Connection_Client::Connection_Client() {

}

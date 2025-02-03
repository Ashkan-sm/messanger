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
int Connection_Client::readmsg(char *out)
{
    memset(out, '\0', strlen(out));
    char header[HEADER_SIZE];
    recv(nClientSocket,header,HEADER_SIZE,0);
    header[HEADER_SIZE]='\0';
    //std::string headstr(header);

    int size=std::atoi(header);
    int o;
    o=recv(nClientSocket,out,size,0);
    out[o]='\0';
//    std::cout<<"got from server: "<<out<<std::endl;
    return o;
}

void Connection_Client::connection_manager(Client* client_pointer) {
    while(1){
        char buffer[4096]={0};
        qInfo()<<"reading";
        readmsg(buffer);
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

void Connection_Client::writemssg(const char *msg) {


    std::string msgstr(msg);

    std::string header(10,' ');
    std::string msglength=std::to_string(msgstr.length());
    header.replace(0,msglength.length(),msglength);

    std::string buffer(header+msgstr);

    send(nClientSocket,buffer.c_str(),buffer.length(),0);

}

void Connection_Client::sendfile(std::string filename, QString to)
{
    std::ifstream file(filename.substr(7),std::ios::binary);
    qInfo()<<filename.substr(7);
    if (file.is_open()){
        writemssg(("/send_file "+filename.substr(filename.rfind('/'))+" "+to.toStdString()).c_str());
        writemssg("EOLOG");



        qInfo()<<filename+" "+to.toStdString();
        long long int size=filesize(filename.substr(7).c_str());
        std::cout<<"size send : "<<size<<std::endl;
        writemssg(std::to_string(size).c_str());

        char buffer[1024];
        do {
            file.read(buffer,sizeof(buffer));
            if (file.gcount()){
                auto a =file.gcount();
                send(nClientSocket,buffer,file.gcount(),0);
            }
        } while (!file.eof());

    }

    else{
        qInfo()<<"didnt open file";
    }
}

std::ifstream::pos_type Connection_Client::filesize(const char *filename) {
        std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
        std::ifstream::pos_type loc=in.tellg();
        in.close();
        return loc;
}

std::string Connection_Client::revcfile(std::string filename) {


        std::ofstream resivedfile("./savedata_client/"+filename);

        ssize_t bytes_received;

        char buffer[1024];
        char sizestr[1024];
        readmsg(sizestr);
        long long int size = atoi(sizestr);
        long long int recvCounter=0;
        while(recvCounter+1024<size){
            bytes_received = recv(nClientSocket, buffer, sizeof(buffer), 0);
            resivedfile.write(buffer,bytes_received);
            recvCounter += bytes_received;
        }
        bytes_received = recv(nClientSocket, buffer, size-recvCounter, 0);
        resivedfile.write(buffer,bytes_received);


        resivedfile.close();

        return filename.substr(filename.find('.')+1);

}

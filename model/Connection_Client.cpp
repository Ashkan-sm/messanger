//
// Created by dev on 1/15/25.
//

#include "Connection_Client.h"

Connection_Client::Connection_Client(int PORT,in_addr_t IP,bool is_udp) {
    port_=PORT;
    ip_=IP;
    is_udp_=is_udp;

    if(!is_udp) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &IP, ipStr, INET_ADDRSTRLEN);
        socket = new zmq::socket_t(context, zmq::socket_type::dealer);
        auto a="tcp://"+ (std::string)ipStr +":" + std::to_string(PORT);
        socket->connect("tcp://"+ (std::string) ipStr +":"+std::to_string(PORT));
    }
    else {

        socket = new zmq::socket_t(context, zmq::socket_type::sub);
        socket->connect("tcp://127.0.0.1:12345");
        socket->set(zmq::sockopt::subscribe, "");   // subscribe to all messages

    }





}

int Connection_Client::read_sock(char *databuf,int size) {
    return read(nClientSocket, databuf,size);
}
messager::Packet Connection_Client::readmsg()
{
    zmq::message_t reply;
    socket->recv(reply);
    messager::Packet packet;
    std::string msg_str(static_cast<char*>(reply.data()), reply.size());
    packet.ParseFromString( msg_str);
    return packet;
}

void Connection_Client::connection_manager(Client* client_pointer) {
    while(1){
        qInfo()<<"reading";
        messager::Packet msg;
        msg=readmsg();
        std::cout<<"got on tcp: "<<msg.DebugString()<<std::endl;

        client_pointer->client_run(msg);



    }
}

Connection_Client::Connection_Client() {

}

void Connection_Client::writemssg(std::string msg) {


    socket->send(zmq::buffer(msg));

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


//        std::ofstream resivedfile("./savedata_client/"+filename);
//
//        ssize_t bytes_received;
//
//        char buffer[1024];
//        char sizestr[1024];
//        readmsg(sizestr);
//        long long int size = atoi(sizestr);
//        long long int recvCounter=0;
//        while(recvCounter+1024<size){
//            bytes_received = recv(nClientSocket, buffer, sizeof(buffer), 0);
//            resivedfile.write(buffer,bytes_received);
//            recvCounter += bytes_received;
//        }
//        bytes_received = recv(nClientSocket, buffer, size-recvCounter, 0);
//        resivedfile.write(buffer,bytes_received);
//
//
//        resivedfile.close();
//
//        return filename.substr(filename.find('.')+1);

}

std::string Connection_Client::recv() {
    zmq::message_t msg;
    socket->recv(msg);

    return std::string{(char*)msg.data(), msg.size()};
}




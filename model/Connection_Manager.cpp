//
// Created by dev on 1/20/25.
//

#include "Connection_Manager.h"

Connection_Manager* Connection_Manager::instance= nullptr;

void Connection_Manager::set_up(int port_tcp,int port_udp,std::string udp_group_ip) {
    cnc_handle=Connection_Server(port_tcp,INADDR_ANY);
    broadcast_handle=Connection_Server(port_udp,inet_addr(udp_group_ip.c_str()),true);
    server = new Server_Obj;

    std::string eth0_ip = cnc_handle.getEth0IPAddress()+" "+std::to_string(cnc_handle.PORT_);
    std::thread multitread([=] {broadcast_handle.send_on_multicast(eth0_ip);});
    multitread.detach();

    nMaxFd = cnc_handle.socket_;

}
void Connection_Manager::pop_user_by_socket(std::vector<User> &list, int sock) {
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].socket_==sock){
            list.erase(list.begin() +i);
        }
    }
}

int Connection_Manager::find_user_id(std::vector<User> &list, std::string id) {
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].id_==id){
            return i;
        }
    }
    return -1;
}
void Connection_Manager::start_server() {
    exec_thread_ = std::make_shared<std::thread>([&]
    {
        while (1) {
            cnc_handle.refresh_socket();
            cnc_handle.ready_socket(cnc_handle.socket_);

            // Add active client sockets to the select sets
            for (int i = 0; i < ArrClient.size(); i++) {
                cnc_handle.ready_socket(ArrClient[i].socket_);
            }
            nRet = cnc_handle.select_socket(nMaxFd);

            if (nRet > 0) {
                // Check for new client connection
                if (int nClientSocket = cnc_handle.accept_connection()) {
                    if (nClientSocket > 0) {
                        int nIndex;
                        // Find an available slot for the new client
                        ArrClient.emplace_back(nClientSocket);
                        std::cout << "Client " << nClientSocket << " connected\n";

                        if (nMaxFd < nClientSocket) {
                            nMaxFd = nClientSocket;
                        }
                        // If the client list is full, reject the connection
                        if (nIndex == MAX_CLIENTS) {
                            std::cout << "Client " << nClientSocket << " connection rejected. Server full\n";
                            close(nClientSocket); // Close the socket if full
                        }
                    }
                }
                for (int i = 0; i < ArrClient.size(); i++) {
                    if (FD_ISSET(ArrClient[i].socket_, &cnc_handle.fr)) {
                        char buff[4096];
                        std::memset(buff, '\0', sizeof(buff));

                        int nRet = cnc_handle.read_msg_from_sock(ArrClient[i].socket_, buff);
                        std::cout<<"buf from client: "<<ArrClient[i].id_<<" : "<<buff<<std::endl;
//                        std::cout << "rescived log: " << buff << std::endl;
                        std::string buffs(buff);

                        if (nRet < 0) {
                            std::cerr << "Error receiving data from client " << ArrClient[i].socket_ << "\n";
                            close(ArrClient[i].socket_);
                            pop_user_by_socket(ArrClient, ArrClient[i].socket_);
                        } else if (nRet == 0) {
                            std::cout << "Client " << ArrClient[i].socket_ << " disconnected\n";
                            close(ArrClient[i].socket_);
                            pop_user_by_socket(ArrClient, ArrClient[i].socket_);
                        } else {
                            if(ArrClient[i].id_!=""){
                                ArrClient[i].logs.push_back(buffs);
                                if (buffs=="EOLOG"){
//                                    std::thread*  tempthread=new std::thread([this,i] {

                                            server->run(this, ArrClient[i].id_, ArrClient[i].logs);

//                                    });
//                                    tempthread->detach();
                                    ArrClient[i].logs.clear();
                                }
                            } else{
                                if(strncmp("/sign_up",buff,strlen("/sign_up"))==0){
                                    std::ifstream username("userid.txt");
                                    int validusername=1;
                                    std::string users;
                                    std::string usern=((std::string)buff).substr(9,((std::string)buff).find(' '));
                                    while (std::getline(username, users)) {
                                        if(usern==users){
                                            validusername=0;
                                        }
                                    }
                                    if(validusername){
                                        std::fstream savefile;
                                        savefile.open("userid.txt",std::ios::in | std::ios::app);
                                        savefile.clear();
                                        savefile.seekp(0, std::ios::end);
                                        savefile << ((std::string)buff).substr(9)+"\n";
                                        savefile.close();
                                    }
                                    username.close();
                                }
                                else{
                                    std::ifstream idfile("userid.txt");
                                    std::ifstream username("userid.txt");

                                    if (idfile.is_open()) {
                                        std::string line;
                                        while (std::getline(idfile, line)) {
                                            if(line == buffs){
                                                {
                                                    {
                                                        ArrClient[i].id_=buffs.substr(0,buffs.find(' '));
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,"Server");
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,("Wellcome "+ArrClient[i].id_).c_str());
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,"EOLOG");
                                                        
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,"/ClientId");
                                                        std::string users;
                                                        while (std::getline(username, users)) {
                                                            cnc_handle.send_msg_to_sock(ArrClient[i].socket_,users.substr(0,users.find(' ')).c_str());
                                                        }
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,"/ClientIdEnd");
                                                        cnc_handle.send_msg_to_sock(ArrClient[i].socket_,"EOLOG");

                                                        //sending personal msgs
                                                        char cwd[1024];  // Buffer to store current working directory
                                                        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                                                            DIR *dir;
                                                            struct dirent *entry;

                                                            dir = opendir(((std::string)cwd+"/resource/savedata_server").c_str());
                                                            while ((entry = readdir(dir)) != nullptr) {
                                                                std::string dname=entry->d_name;
                                                                std::string foruser=dname.substr(0,dname.find(' '));

                                                                if (foruser==ArrClient[i].id_){
                                                                    std::string touser=dname.substr(dname.find(' ')+1,dname.find(".txt")-dname.find(' ')-1);
                                                                    std::ifstream file("resource/savedata_server/"+(std::string)entry->d_name);
                                                                    if (file.is_open()){
                                                                        std::string line="";

                                                                        std::string sendbuffer="";
                                                                        while (std::getline(file,line)) {
                                                                            sendbuffer+=line+"\n";
                                                                            if (line.find("EOMSG") != std::string::npos){
                                                                                cnc_handle.send_msg_to_sock(ArrClient[i].socket_,sendbuffer.substr(0,sendbuffer.length()-strlen("EOMSG")-1).c_str());
                                                                                sendbuffer="";
                                                                            }
                                                                        }
                                                                        file.close();

                                                                        int index= find_user_id(ArrClient,touser);
                                                                        if(index < MAX_CLIENTS){
                                                                            cnc_handle.send_msg_to_sock(ArrClient[index].socket_,("/status_update "+ArrClient[i].id_+" 2").c_str());
                                                                            cnc_handle.send_msg_to_sock(ArrClient[index].socket_,"EOLOG");
                                                                            std::cout<<"sent :2 to user: "<<ArrClient[index].id_<<std::endl;
                                                                        }
                                                                    }
                                                                    remove(("resource/savedata_server/"+(std::string)entry->d_name).c_str());
                                                                }
                                                            }

                                                            closedir(dir);
                                                        }


                                                    }
                                                }
                                            }

                                        }
                                        if(ArrClient[i].id_==""){
                                            // char msg1[]="Server";
                                            // cnc_handle.send_msg_to_sock(ArrClient[i].socket_,msg1);
                                            // char msg2[]="Invalid Login";
                                            // cnc_handle.send_msg_to_sock(ArrClient[i].socket_,msg2);
                                        }
                                        idfile.close();  // Close the file
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
    });
}

void Connection_Manager::send_message_(std::string client_id, std::string message) {
    std::cout<<"sending to client: "<<client_id << "_" << message <<std::endl;
    cnc_handle.send_msg_to_sock(ArrClient[find_user_id(ArrClient,client_id)].socket_,message.c_str());
}

void Connection_Manager::logout(std::string id) {
    ArrClient[find_user_id(ArrClient,id)].id_="";
}

void Connection_Manager::send_file_(std::string client_id, std::string message) {
    cnc_handle.send_file_to_sock(ArrClient[find_user_id(ArrClient,client_id)].socket_,message);
}

void Connection_Manager::read_file_(std::string client_id, std::string message) {
    cnc_handle.read_file_from_sock(ArrClient[find_user_id(ArrClient,client_id)].socket_,message);
}

void Connection_Manager::join_exec() {
    exec_thread_->join();
}

Connection_Manager& Connection_Manager::get_instance() {
    if (instance== nullptr){
        instance=new Connection_Manager();
    }
    return (*instance);
}

Connection_Manager::Connection_Manager() {

}

std::vector<User> &Connection_Manager::getArrClient() {
    return ArrClient;
}



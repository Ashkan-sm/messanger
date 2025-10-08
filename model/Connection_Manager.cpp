//
// Created by dev on 1/20/25.
//

#include "Connection_Manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void ensureFileExists(const std::string& path) {
    fs::path filePath(path);
    fs::path dirPath = filePath.parent_path();

    // Create directories recursively if they do not exist
    if (!dirPath.empty() && !fs::exists(dirPath)) {
        if (!fs::create_directories(dirPath)) {
            throw std::runtime_error("Failed to create directories: " + dirPath.string());
        }
    }

    // Create the file if it does not exist
    if (!fs::exists(filePath)) {
        std::ofstream ofs(filePath);
        if (!ofs) {
            throw std::runtime_error("Failed to create file: " + filePath.string());
        }
        ofs.close();
    }
}
Connection_Manager* Connection_Manager::instance= nullptr;

void Connection_Manager::set_up(int port_tcp,int port_udp,std::string udp_group_ip) {


    std::string path = "resource/savedata_server/";
    try {
        ensureFileExists(path);
        std::cout << "File exists or was created: " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    cnc_handle=new Connection_Server(port_tcp,INADDR_ANY);
    broadcast_handle=new Connection_Server(port_udp,inet_addr(udp_group_ip.c_str()),true);
    server = new Server_Obj;

    std::string eth0_ip = cnc_handle->getEth0IPAddress();
    if (eth0_ip==""){
        std::cerr<<"no interface found"<<std::endl;
        exit(1);
    }
    eth0_ip += " "+std::to_string(cnc_handle->PORT_);
    std::thread multitread([=] {broadcast_handle->send_on_multicast(eth0_ip);});
    multitread.detach();



}
void Connection_Manager::pop_user_by_socket(std::vector<User> &list, int sock) {
    for (int i = 0; i < list.size(); ++i) {
        if (list[i].socket_==sock){
            list.erase(list.begin() +i);
        }
    }
}


void Connection_Manager::start_server() {
    exec_thread_ = std::make_shared<std::thread>([&] {
        while (1) {

            std::pair<std::string, std::string> msg = cnc_handle->recv();
            std::string client_id = msg.first;
            std::string req = msg.second;

            std::cout << "buf from client: " << client_id << " : " << req << std::endl;
            ArrClient[client_id];
            if (strncmp("/sign_up", req.c_str(), strlen("/sign_up")) == 0) {
                std::ifstream username("userid.txt");
                int validusername = 1;
                std::string users;
                std::string usern = req.substr(9, req.find(' '));
                while (std::getline(username, users)) {
                    if (usern == users) {
                        validusername = 0;
                    }
                }
                if (validusername) {
                    std::fstream savefile;
                    savefile.open("userid.txt", std::ios::in | std::ios::app);
                    savefile.clear();
                    savefile.seekp(0, std::ios::end);
                    savefile << req.substr(9) + "\n";
                    std::cout << "added user" << std::endl;
                    savefile.close();
                }
                username.close();

            } else {
                if (ArrClient[client_id].authenticated){
                    server->run(this, client_id, ArrClient[client_id].logs);
                }
                std::ifstream idfile("userid.txt");
                std::ifstream username("userid.txt");

                if (idfile.is_open()) {
                    std::string line;
                    while (std::getline(idfile, line)) {
                        if (line == req) {
                            ArrClient[client_id].authenticated = true;
                            cnc_handle->send_msg_to_sock(client_id, "Server");
                            cnc_handle->send_msg_to_sock(client_id, ("Wellcome " + client_id).c_str());
                            cnc_handle->send_msg_to_sock(client_id, "EOLOG");

                            cnc_handle->send_msg_to_sock(client_id, "/ClientId");
                            std::string users;
                            while (std::getline(username, users)) {
                                cnc_handle->send_msg_to_sock(client_id, users.substr(0, users.find(' ')).c_str());
                            }
                            cnc_handle->send_msg_to_sock(client_id, "/ClientIdEnd");
                            cnc_handle->send_msg_to_sock(client_id, "EOLOG");

                            //sending personal msgs
                            char cwd[1024];  // Buffer to store current working directory
                            if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                                DIR *dir;
                                struct dirent *entry;

                                dir = opendir(((std::string) cwd + "/resource/savedata_server").c_str());
                                while ((entry = readdir(dir)) != nullptr) {
                                    std::string dname = entry->d_name;
                                    std::string foruser = dname.substr(0, dname.find(' '));

                                    if (foruser == client_id) {
                                        std::string touser = dname.substr(dname.find(' ') + 1,
                                                                          dname.find(".txt") - dname.find(' ') - 1);
                                        std::ifstream file("resource/savedata_server/" + (std::string) entry->d_name);
                                        if (file.is_open()) {
                                            std::string line = "";

                                            std::string sendbuffer = "";
                                            while (std::getline(file, line)) {
                                                sendbuffer += line + "\n";
                                                if (line.find("EOMSG") != std::string::npos) {
                                                    cnc_handle->send_msg_to_sock(client_id, sendbuffer.substr(0,
                                                                                                              sendbuffer.length() -
                                                                                                              strlen("EOMSG") -
                                                                                                              1).c_str());
                                                    sendbuffer = "";
                                                }
                                            }
                                            file.close();

                                            if (ArrClient.count(touser)) {
                                                cnc_handle->send_msg_to_sock(touser, ("/status_update " + client_id +
                                                                                      " 2").c_str());
                                                cnc_handle->send_msg_to_sock(touser, "EOLOG");
                                                std::cout << "sent :2 to user: " << touser << std::endl;
                                            }
                                        }
                                        remove(("resource/savedata_server/" + (std::string) entry->d_name).c_str());
                                    }
                                }

                                closedir(dir);
                            }

                        }

                    }
                }
                idfile.close();
            }
        }
    });
}

void Connection_Manager::send_message_(std::string client_id, std::string message) {
    std::cout<<"sending to client: "<<client_id << "_" << message <<std::endl;
    cnc_handle->send_msg_to_sock(client_id,message.c_str());
}

void Connection_Manager::logout(std::string id) {
    ArrClient[id].authenticated=false;
}

void Connection_Manager::send_file_(std::string client_id, std::string message) {
//    cnc_handle->send_file_to_sock(client_id,message);
}

void Connection_Manager::read_file_(std::string client_id, std::string message) {
//    cnc_handle.read_file_from_sock(ArrClient[find_user_id(ArrClient,client_id)].socket_,message);
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

std::map<std::string,User> &Connection_Manager::getArrClient() {
    return ArrClient;
}



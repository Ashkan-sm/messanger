//
// Created by dev on 1/15/25.
//

#include "Server_Obj.h"


Server_Obj::Server_Obj() {

}

std::string Server_Obj::find_user_id(std::map<std::string,User> &list, std::string name) {
    for (auto i:list) {
        if (i.second.name_==name){
            return i.first;
        }
    }
    return "";
}
void Server_Obj::run(Connection_Manager* conection_manager,std::string id, messager::Packet &msg) {
   if(msg.packet_type()==messager::Packet_PacketType_PACKET_TYPE_FILE_SEND){
//        std::string filename=log_arr[0].substr(log_arr[0].find(' ')+1);
//        std::string foruser=filename.substr(filename.find(' ')+1);
//        filename=filename.substr(0,filename.find(' '));
//        std::cout<<"chrlinh names: "<<filename<<std::endl;
//        conection_manager->read_file_(id,filename);
    }

    else if(msg.packet_type()==messager::Packet_PacketType_PACKET_TYPE_PRIVATE_MESSAGES){
        std::string recname=msg.receiver_name();
        std::cout<<"recname on meesaage: "<<recname<<std::endl;
        std::string recid= find_user_id(conection_manager->getArrClient(),recname);

        if (recid!=""){
            if(recid!=id){
                messager::Packet send_msg;
                send_msg.set_data(msg.data());
                send_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_PRIVATE_MESSAGES);

                conection_manager->send_message_(recid,send_msg.SerializeAsString());

            }

            messager::Packet send_msg;
            send_msg.set_data("2");
            send_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE);
            conection_manager->send_message_(recid,send_msg.SerializeAsString());

        }
        else{

            messager::Packet send_msg;
            send_msg.set_data("1");
            send_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE);
            conection_manager->send_message_(recid,send_msg.SerializeAsString());

            std::fstream savefile;

            savefile.open("resource/savedata_server/"+recname+" "+id+".txt", std::ios::out | std::ios::app);

            // Check if the file is open
            if (savefile.is_open()) {

                savefile << msg.SerializeAsString();
                savefile << "EOMSG\n";
                savefile << "EOLOGEOMSG\n";

                savefile.close();
            } else {
                std::cerr << "Failed to open the file.\n";
            }
        }


    }
    else if(msg.packet_type()==messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE){
        std::string recname=msg.receiver_name();
        std::string status=msg.data();
        std::string recid= find_user_id(conection_manager->getArrClient(),recname);

        if (recid!=""){
            messager::Packet send_msg;
            send_msg.set_data(status);
            send_msg.set_packet_type(messager::Packet_PacketType_PACKET_TYPE_STATUS_UPDATE);
            conection_manager->send_message_(recid,send_msg.SerializeAsString());


        }
        else{
            std::fstream savefile;
            savefile.open("resource/savedata_server/"+recname+" "+id+".txt", std::ios::out | std::ios::app);
            // Check if the file is open
            if (savefile.is_open()) {
                // Write to the file
                savefile << msg.SerializeAsString();
                savefile << "EOMSG\n";
                savefile << "EOLOGEOMSG\n";
                // Close the file after writing
                savefile.close();
            } else {
                std::cerr << "Failed to open the file.\n";
            }
        }
    }
    else if(msg.packet_type()==messager::Packet_PacketType_PACKET_TYPE_FILE_GET){
//        conection_manager->send_file_(id,msg.data());
    }
    else if(msg.packet_type()==messager::Packet_PacketType_PACKET_TYPE_LOGOUT){
        std::cout<<"loged out user: "<<id<<std::endl;
        conection_manager->logout(id);
    }
}







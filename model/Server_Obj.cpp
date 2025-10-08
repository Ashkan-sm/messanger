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
void Server_Obj::run(Connection_Manager* conection_manager,std::string id,std::vector<std::string> log_arr) {

   if(log_arr[0].compare(0, strlen("/send_file"),"/send_file")==0){
        std::string filename=log_arr[0].substr(log_arr[0].find(' ')+1);
        std::string foruser=filename.substr(filename.find(' ')+1);
        filename=filename.substr(0,filename.find(' '));
        std::cout<<"chrlinh names: "<<filename<<std::endl;
        conection_manager->read_file_(id,filename);
    }

    else if(log_arr[0].compare(0, strlen("/private_msg"),"/private_msg")==0){
        std::string recname=log_arr[0].substr(log_arr[0].find(' ')+1);
        std::cout<<"recname on meesaage: "<<recname<<std::endl;
        std::string recid= find_user_id(conection_manager->getArrClient(),recname);

        if (recid!=""){
            if(recid!=id){
                conection_manager->send_message_(recid, "/private_msg " + id);
                conection_manager->send_message_(recid,log_arr[1]);
                conection_manager->send_message_(recid,"EOLOG");
            }
            conection_manager->send_message_(id,"/status_update "+recname+" 2");
            conection_manager->send_message_(id,"EOLOG");

        }
        else{
            conection_manager->send_message_(id,"/status_update "+recname+" 1");
            conection_manager->send_message_(id,"EOLOG");


            std::fstream savefile;

            savefile.open("resource/savedata_server/"+recname+" "+id+".txt", std::ios::out | std::ios::app);

            // Check if the file is open
            if (savefile.is_open()) {
                // Write to the file

                savefile << "/private_msg "+id;
                savefile << "EOMSG\n";
                savefile << log_arr[1];
                savefile << "EOMSG\n";

                savefile << "EOLOGEOMSG\n";
                // Close the file after writing
                savefile.close();
            } else {
                std::cerr << "Failed to open the file.\n";
            }
        }


    }
    else if(log_arr[0].compare(0, strlen("/status_update"),"/status_update")==0){
        std::string recname=log_arr[0].substr(log_arr[0].find(' ')+1);
        char status=recname[recname.length()-1];
        recname=recname.substr(0,recname.find(' '));
        std::string recid= find_user_id(conection_manager->getArrClient(),recname);
        if (recid!=""){
            conection_manager->send_message_(recid,"/status_update "+id+" "+status);
            conection_manager->send_message_(recid,"EOLOG");

        }
        else{
            std::fstream savefile;
            savefile.open("resource/savedata_server/"+recname+" "+id+".txt", std::ios::out | std::ios::app);
            // Check if the file is open
            if (savefile.is_open()) {
                // Write to the file
                savefile << log_arr[0];
                savefile << "EOMSG\n";
                savefile << "EOLOGEOMSG\n";
                // Close the file after writing
                savefile.close();
            } else {
                std::cerr << "Failed to open the file.\n";
            }
        }
    }
    else if(log_arr[0].compare(0, strlen("/get_file"),"/get_file")==0){
        conection_manager->send_file_(id,log_arr[0].substr(log_arr[0].find(" ")+1));

    }
    else if(log_arr[0].compare(0, strlen("/logout"),"/logout")==0){
        std::cout<<"loged out user: "<<id<<std::endl;
        conection_manager->logout(id);
    }
}







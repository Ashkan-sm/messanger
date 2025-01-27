//
// Created by dev on 1/15/25.
//

#ifndef CHATROOM_FILE_HANDLE_H
#define CHATROOM_FILE_HANDLE_H

#include <string>
#include <fstream>

class File_Handle {
    std::string path_;
    char method_;

public:
    File_Handle(std::string path,char method);
    std::fstream file_;
    long long int size_of_file(std::string path);
};


#endif //CHATROOM_FILE_HANDLE_H

//
// Created by dev on 1/15/25.
//

#ifndef CHATROOM_FILE_HANDLE_H
#define CHATROOM_FILE_HANDLE_H

#include <string>
#include <fstream>
#include <iostream>
class File_Handle {
private:
    std::string path_;

public:
    std::fstream file_;
    enum Method{IN,OUT,APP,BINARY};
    File_Handle(std::string path,Method);
    ~File_Handle();
    std::basic_istream<char>& readline(std::string &);
    void write(std::string);
    int IsOpen();
    long long int size_of_file(std::string path);
};


#endif //CHATROOM_FILE_HANDLE_H

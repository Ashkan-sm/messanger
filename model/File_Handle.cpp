//
// Created by dev on 1/15/25.
//

#include "File_Handle.h"

File_Handle::File_Handle(std::string path, Method method) {
    path_=path;

    switch (method) {
        case Method::IN:
            file_.open(path,std::ios::binary | std::ios::in);
            break;
        case Method::OUT:
            file_.open(path,std::ios::out);
            break;
        case Method::APP:
            file_.open(path,std::ios::in | std::ios::app);

            file_.clear();
            file_.seekp(0, std::ios::end);

    }


}

long long int File_Handle::size_of_file(std::string path) {
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

std::basic_istream<char>& File_Handle::readline(std::string & s) {
    return std::getline(file_,s);
}

File_Handle::~File_Handle() {
    file_.close();
    std::cout<<"kiled filed";
}

void File_Handle::write(std::string S) {
    file_ << S;
}

int File_Handle::IsOpen() {
    return file_.is_open();
}

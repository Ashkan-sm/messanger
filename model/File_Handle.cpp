//
// Created by dev on 1/15/25.
//

#include "File_Handle.h"

File_Handle::File_Handle(std::string path, char method) {
    path_=path;
    method_=method;

    switch (method) {
        case 'b':
            file_.open(path,std::ios::binary | std::ios::in);
            break;
        case 'w':
            file_.open(path,std::ios::out);
            break;
    }

}

long long int File_Handle::size_of_file(std::string path) {
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

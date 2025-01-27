#include "model/Connection_Manager.h"
#include "model/Server_Obj.h"
#include "model/Connection_Manager.h"


#define HEADER_SIZE 10


int main() {
    Connection_Manager server1(5050,12345,"226.1.1.1");
    server1.start_server();
    server1.join_exec();


    return 0;
}

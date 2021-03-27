#include "ftpserver.h"


int main(int argc, char** argv) {
    // it is not necessary until add read config.json
    unsigned short commandOffset = 1; 
    unsigned int port = 4242;
    std::string dir = "./"; 

    server* myServer = new server(port, dir, commandOffset);

    delete myServer;

    return (EXIT_SUCCESS);
}

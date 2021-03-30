#include "server.h"

server::server(uint commandChannelPort, uint dataChannelPort, string dir, unsigned short commandOffset) : dir(dir), commandOffset(commandOffset), shutdown(false), connId(0) {
    this->dataChannelPort = dataChannelPort;
    this->initSockets(commandChannelPort);
    this->Connection();
}


int server::Connection() {
    int fd; 
    this->cli_size = sizeof(this->cli);
    fd = accept(this->s, (struct sockaddr*) &cli, &cli_size);
    if (fd < 0) {
        cerr << "Error while accepting client" << endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}


void server::initSockets(int commandChannelPort) {
    int reuseAllowed = 1;
    this->maxClients = 5;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(commandChannelPort);
    this->addr.sin_addr.s_addr = INADDR_ANY;
    this->s = socket(PF_INET, SOCK_STREAM, 0);
    if (this->s == -1) {
        cerr << "socket() failed" << endl;
        return;
    }
    else if (setsockopt(this->s, SOL_SOCKET, SO_REUSEADDR, &reuseAllowed, sizeof(reuseAllowed)) < 0) {
        cerr << "setsockopt() failed" << endl;
        close (this->s);
        return;
    }
    if (bind(this->s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        cerr << ("bind() failed (do you have the apropriate rights? is the port unused?)") << endl;
        close (this->s);
        return;
    }
    else if (listen(this->s, this->maxClients) == -1) {
        cerr << ("listen () failed") << endl;
        close (this->s);
        return;
    }
}

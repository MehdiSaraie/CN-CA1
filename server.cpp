#include "server.h"

server::server(uint commandChannelPort, uint dataChannelPort, string dir, unsigned short commandOffset) : commandChannelPort(commandChannelPort), dataChannelPort(dataChannelPort), dir(dir), commandOffset(commandOffset), shutdown(false), connId(0) {
    this->initSockets(commandChannelPort);
}

int server::Accept() {
    int data_socket; 
    this->cli_size = sizeof(this->cli);
    data_socket = accept(this->listening_socket, (struct sockaddr*) &cli, &cli_size);
    if (data_socket < 0) {
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
    this->listening_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (this->listening_socket == -1) {
        cerr << "socket() failed" << endl;
        return;
    }
    else if (setsockopt(this->listening_socket, SOL_SOCKET, SO_REUSEADDR, &reuseAllowed, sizeof(reuseAllowed)) < 0) {
        cerr << "setsockopt() failed" << endl;
        close (this->listening_socket);
        return;
    }
    if (bind(this->listening_socket, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1) {
        cerr << ("bind() failed (do you have the apropriate rights? is the port unused?)") << endl;
        close (this->listening_socket);
        return;
    }
    else if (listen(this->listening_socket, this->maxClients) == -1) {
        cerr << ("listen () failed") << endl;
        close (this->listening_socket);
        return;
    }
}

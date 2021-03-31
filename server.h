#ifndef _SERVER_H
#define	_SERVER_H

// #include "serverconnection.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class server {
public:
    server(uint commandChannelPort, uint dataChannelPort, string dir, unsigned short commandOffset = 1);
private:
    void initSockets(int commandChannelPort);
    int Accept();
    int commandChannelPort;
    int dataChannelPort;
    unsigned int maxClients;
    int listening_socket;
    fd_set socks;
    string dir;
    unsigned int connId;
    bool shutdown;
    struct sockaddr_in addr;
    struct sockaddr_in cli;
    socklen_t cli_size;
    unsigned short commandOffset;
};

#endif


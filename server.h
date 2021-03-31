#ifndef _SERVER_H
#define	_SERVER_H

// #include "serverconnection.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

class server {
public:
    server(uint commandChannelPort, uint dataChannelPort, string dir, unsigned short commandOffset = 1);
	void Run();
private:
    void InitSockets(int commandChannelPort);
    int commandChannelPort;
    int dataChannelPort;
    int listening_socket;
    fd_set readfds;
    string dir;
    unsigned int connId;
    bool shutdown;
    struct sockaddr_in addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_size;
    unsigned short commandOffset;
};

#endif


#ifndef _CLIENT_H
#define	_CLIENT_H

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

class client {
public:
    client(uint commandChannelPort, uint dataChannelPort, string dir);

private:
    void initSockets(int commandChannelPort);
    int dataChannelPort;
    int s;
    // fd_set socks;
    string dir;
    struct sockaddr_in addr;
    struct sockaddr_in srv;
    socklen_t cli_size;
    unsigned short commandOffset;
};

#endif


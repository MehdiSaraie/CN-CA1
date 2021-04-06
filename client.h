#ifndef _CLIENT_H
#define	_CLIENT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <fstream>

using namespace std;

class client {
public:
	client(uint commandChannelPort);
	void Communicate(uint dataChannelPort);
private:
	int InitSocket(int port);
	int sock;
};

#endif


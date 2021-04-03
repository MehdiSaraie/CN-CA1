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
#include <jsoncpp/json/json.h>
#include <fstream>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <ctime>
#include <dirent.h>


#define MAXCLIENTS 10

using namespace std;

struct connected_client {
	int socket;
	int login;
	string user;
	string password;
	bool admin;
	int size;
	string dir;
};

class server {
public:
	server(uint commandChannelPort, uint dataChannelPort, string dir, Json::Value config);
	void Run();
private:
	void InitSockets(int commandChannelPort);
	void WriteInFile(string user, string action, string input1 = "", string input2 = "");
	int commandChannelPort;
	int dataChannelPort;
	Json::Value config;
	int listening_socket;
	fd_set readfds;
	struct connected_client clients[MAXCLIENTS];
	string dir;
	struct sockaddr_in addr;
	struct sockaddr_in cli_addr;
	socklen_t cli_size;
	ofstream outfile;
	time_t now;
    char* date_time;

};

#endif


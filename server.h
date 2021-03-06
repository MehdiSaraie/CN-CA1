#ifndef _SERVER_H
#define	_SERVER_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
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
	int command_socket;
	int login;
	string user;
	string password;
	bool admin;
	int size;
	string dir;
};

class server {
public:
	server(uint commandChannelPort, uint dataChannelPort, Json::Value config);
	void Run();
private:
	int InitSocket(int port);
	void WriteInFile(string user, string action, string input1 = "", string input2 = "");
	vector<string> validDir(string dir);
	bool isPossible(string dir_path);
	bool fileAvailibility(string filename);
	Json::Value config;
	int command_listening_socket;
	int data_listening_socket;
	fd_set readfds;
	struct connected_client clients[MAXCLIENTS];
	struct sockaddr_in cli_addr;
	socklen_t cli_size;
	ofstream outfile;
	time_t now;
	char* date_time;

};

#endif


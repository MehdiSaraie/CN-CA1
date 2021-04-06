#include "client.h"
#define BUFSIZE 1024

client::client(uint commandChannelPort) {
	this->sock = this->InitSocket(commandChannelPort);
}


int client::InitSocket(int port) {
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cerr << "socket() failed" << endl;
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		perror("Connection Failed");
		close(sock);
		return -1;
	}
	return sock;
}


void client::Communicate(uint dataChannelPort) {
	string input;
	char* message;
	while(true) {
		getline(cin,input);
		message = &input[0];
		if (send(this->sock, message, strlen(message), 0) != strlen(message))
			cerr << ("send() sent a different number of bytes than expected");

		char buffer[BUFSIZE+1];
		memset(&buffer, 0, BUFSIZE);
		int valread = recv(this->sock, buffer, BUFSIZE, 0);
		if (valread > 0) {
			puts(buffer);
			if (strncmp(buffer, "226", 3) == 0) {
				int data_sock = this->InitSocket(dataChannelPort);
				memset(&buffer, 0, BUFSIZE);
				recv(data_sock, buffer, BUFSIZE, 0);
				puts(buffer);
				close(data_sock);
			}
		}	
	}
}

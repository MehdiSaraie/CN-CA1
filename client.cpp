#include "client.h"
#define BUFSIZE 1024

client::client(uint commandChannelPort, uint dataChannelPort, string dir) : dataChannelPort(dataChannelPort) {
    this->InitSockets(commandChannelPort);
}


void client::InitSockets(int commandChannelPort) {
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(commandChannelPort);
    //this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->s = socket(AF_INET, SOCK_STREAM, 0);
    if (this->s == -1) {
        cerr << "socket() failed" << endl;
        return;
    }
    // ????????????????????????????????
    // if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
    //     perror("\nInvalflag address/ Address not supported \n"); 
    // } 
   
    if (connect(this->s, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr)) < 0) { 
        perror("\nConnection Failed \n");
    }
}


void client::Communicate() {
	string input;
	char* message;
	while(true) {
		getline(cin,input);
		message = &input[0];
		if (send(this->s, message, strlen(message), 0) != strlen(message))
			cerr << ("send() sent a different number of bytes than expected");

        char buffer[BUFSIZE+1];
        memset(&buffer, 0, BUFSIZE);
        if (recv(this->s, buffer, BUFSIZE, 0) > 0) {
            puts(buffer);
        }
	}

}

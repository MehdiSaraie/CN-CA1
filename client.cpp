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
            if(input == "ls"){
                this->createDataChannel();
            }
        }
	}

}

void client::createDataChannel(){
    int sock = 0, valread;
    struct sockaddr_in serv_addrData;
    char buffer[BUFSIZE] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
    }
   
    serv_addrData.sin_family = AF_INET;
    serv_addrData.sin_port = htons(this->dataChannelPort);

    if (connect(sock, (struct sockaddr *)&serv_addrData, sizeof(serv_addrData)) < 0){
        printf("\nConnection Failed \n");
    }
    valread = read(sock , buffer, BUFSIZE);
    cout << buffer << endl;

}


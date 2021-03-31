#include "client.h"

client::client(uint commandChannelPort, uint dataChannelPort, string dir) : dataChannelPort(dataChannelPort) {
    this->initSockets(commandChannelPort);
}


void client::initSockets(int commandChannelPort) {
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

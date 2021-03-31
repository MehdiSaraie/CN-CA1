#include "server.h"
#define BUFSIZE 1024
#define MAXCLIENTS 5

server::server(uint commandChannelPort, uint dataChannelPort, string dir, unsigned short commandOffset) : commandChannelPort(commandChannelPort), dataChannelPort(dataChannelPort), dir(dir), commandOffset(commandOffset), shutdown(false), connId(0) {
    this->InitSockets(commandChannelPort);
}


void server::InitSockets(int commandChannelPort) {
    int reuseAllowed = 1;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(commandChannelPort);
    this->addr.sin_addr.s_addr = INADDR_ANY;
    this->listening_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (this->listening_socket == -1) {
        cerr << "socket() failed" << endl;
        return;
    }
    else if (setsockopt(this->listening_socket, SOL_SOCKET, SO_REUSEADDR, &reuseAllowed, sizeof(reuseAllowed)) < 0) {
        cerr << "setsockopt() failed" << endl;
        close (this->listening_socket);
        return;
    }
    if (bind(this->listening_socket, (struct sockaddr*)&this->addr, sizeof(this->addr)) == -1) {
        cerr << ("bind() failed (do you have the apropriate rights? is the port unused?)") << endl;
        close (this->listening_socket);
        return;
    }
    else if (listen(this->listening_socket, MAXCLIENTS) == -1) {
        cerr << ("listen () failed") << endl;
        close (this->listening_socket);
        return;
    }
}


void server::Run() {

	int new_socket, client_socket[MAXCLIENTS], activity, i, valread, sd, max_sd;
	
	for (i = 0; i < MAXCLIENTS; i++)
    {
        client_socket[i] = 0;
    }

	while(true) {

		FD_ZERO(&this->readfds);

        FD_SET(this->listening_socket, &this->readfds);
        max_sd = this->listening_socket;
             
        //add child sockets to set
        for (i = 0; i < MAXCLIENTS; i++) {
            //socket descriptor 
            sd = client_socket[i];
                 
            //if valid socket descriptor then add to read list 
            if (sd > 0)
                FD_SET(sd, &this->readfds);
                 
            //highest file descriptor number, need it for the select function 
            if (sd > max_sd)
                max_sd = sd;
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(max_sd + 1, &this->readfds, NULL, NULL, NULL);
       
        if ((activity < 0) && (errno!=EINTR)) {
            cout << "select error\n";
        }

        //If something happened on the master socket then its an incoming connection
        if (FD_ISSET(this->listening_socket, &this->readfds)) {
            if ((new_socket = accept(this->listening_socket, (struct sockaddr *)&this->cli_addr, (socklen_t*)&this->cli_size))<0) {
                cerr << ("Error while accepting client\n");
                return exit(EXIT_FAILURE);
            }  

            //inform user of socket number - used in send and receive commands
            cout << "New connection with socket fd : " << new_socket << ", ip : " << inet_ntoa(this->cli_addr.sin_addr) << ", port :" << ntohs(this->cli_addr.sin_port) << endl;

            //add new socket to array of sockets
            for (i = 0; i < MAXCLIENTS; i++) {
                //if position is empty
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    cout << "Adding to list of sockets as " << i << endl;
                    break;
                }
            }
        }
             
        //else its some IO operation on some other socket
        for (i = 0; i < MAXCLIENTS; i++) {
            sd = client_socket[i];  

            if (FD_ISSET(sd, &this->readfds)) {
				char buffer[BUFSIZE+1];
                //Check if it was for closing , and also read the
                //incoming message
				valread = recv(sd, buffer, BUFSIZE, 0);
                if (valread==0) {
                    //Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr*)&this->cli_addr, (socklen_t*)&this->cli_size);
                    cout << "Host disconnected with ip : " << inet_ntoa(this->cli_addr.sin_addr) << ", port : " <<  ntohs(this->cli_addr.sin_port) << endl;

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }
                     
                else {
					string command, arg;
					buffer[valread] = '\0';
                    istringstream stream(buffer);
					cout << buffer << endl;
					stream >> command;
					while(stream >> arg) {
						cout << arg << endl;
					}

					//prepare response here
                    //send response
                }
            }
        }


		//if ((valread = recv(client_Socket, buffer, BUFSIZE, 0)) < 0)
		//	DieWithError("recv() failed");
		/* Send received string and receive again until end of transmission */
		//while (valread > 0) { /* zero indicates end of transmission */
		//	if ((valread = recv(client_socket, buffer, BUFSIZE, 0)) < 0)
		//		DieWithError("recv() failed");

		
	}
}

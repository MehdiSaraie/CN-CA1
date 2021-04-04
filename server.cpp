#include "server.h"
#define BUFSIZE 1024

server::server(uint commandChannelPort, uint dataChannelPort, string dir, Json::Value config) : commandChannelPort(commandChannelPort), dataChannelPort(dataChannelPort), dir(dir), config(config) {
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
    else if (listen(this->listening_socket, 3) == -1) {
        cerr << ("listen () failed") << endl;
        close (this->listening_socket);
        return;
    }

	for (int i = 0; i < MAXCLIENTS; i++)
    {
        this->clients[i].socket = 0;
    }
}


void server::Run() {

	int new_socket, activity, i, valread, sd, max_sd;

	while(true) {

		FD_ZERO(&this->readfds);

        FD_SET(this->listening_socket, &this->readfds);
        max_sd = this->listening_socket;
             
        //add child sockets to set
        for (i = 0; i < MAXCLIENTS; i++) {
            //socket descriptor 
            sd = this->clients[i].socket;
                 
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
            cout << "select error";
        }

        //If something happened on the master socket then its an incoming connection
        if (FD_ISSET(this->listening_socket, &this->readfds)) {
            if ((new_socket = accept(this->listening_socket, (struct sockaddr *)&this->cli_addr, (socklen_t*)&this->cli_size))<0) {
                cerr << ("Error while accepting client");
                return exit(EXIT_FAILURE);
            }  

            //inform user of socket number - used in send and receive commands
            cout << "New connection with socket fd: " << new_socket << ", ip: " << inet_ntoa(this->cli_addr.sin_addr) << ", port: " << ntohs(this->cli_addr.sin_port) << endl;

            //add new socket to array of sockets
            for (i = 0; i < MAXCLIENTS; i++) {
                //if position is empty
                if (this->clients[i].socket == 0) {
                    this->clients[i].socket = new_socket;
					this->clients[i].login = 0;
					this->clients[i].user = "";
                    this->clients[i].dir = "./";
                    cout << "Adding to list of sockets as " << i << endl;
                    break;
                }
            }
			
        }
             
        //else its some IO operation on some other socket
        for (i = 0; i < MAXCLIENTS; i++) {
            sd = this->clients[i].socket;  

            if (FD_ISSET(sd, &this->readfds)) {
				char buffer[BUFSIZE+1];
                //Check if it was for closing , and also read the
                //incoming message
				valread = recv(sd, buffer, BUFSIZE, 0);
                if (valread==0) {
                    //Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr*)&this->cli_addr, (socklen_t*)&this->cli_size);
                    cout << "Host disconnected with ip: " << inet_ntoa(this->cli_addr.sin_addr) << ", port: " <<  ntohs(this->cli_addr.sin_port) << endl;

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    this->clients[i].socket = 0;
                }
                     
                else {
					string command, arg, response;
					vector<string> args;
					buffer[valread] = '\0';
                    istringstream stream(buffer);
					stream >> command;
					while(stream >> arg) {
						args.push_back(arg);
					}

				    //prepare response here

					if (command == "user") {

						if (this->clients[i].login == 2)
							response = "please quit first";

						else if (args.size() != 1)
							response = "501: Syntax error in parameters or arguments.";

						else {
							string username=args[0];
							Json::Value users = this->config["users"];
							for (int j = 0; j < users.size(); j++) {
								if (users[j]["user"] == username) {
									response = "331: User name okay, need password";
									this->clients[i].login = 1;
									this->clients[i].password = users[j]["password"].asString();
									this->clients[i].admin = users[j]["admin"] == "true";
									this->clients[i].size = stoi(users[j]["size"].asString());
									break;
								}
							}
							this->clients[i].user = username;
						}

					}

					else if (command == "pass") {

						if (this->clients[i].login == 2)
							response = "please quit first";

						else if (this->clients[i].login == 0) {
							if (this->clients[i].user == "")
								response = "503: Bad sequence of commands.";
							else
								response = "430: Invalid username or password";
						}

						else if (args.size() != 1)
							response = "501: Syntax error in parameters or arguments.";

						else {
							string password = args[0];
							if (password == this->clients[i].password) {
								response = "230: User logged in, proceed. Logged out if appropriate.";
								this->clients[i].login = 2;
                                this->WriteInFile(this->clients[i].user, "Logged In");
							}
							else
								response = "430: Invalid username or password";
						}

					}

                    else if (this->clients[i].login == 2){
                        if (command == "pwd"){
                            response = ("257: " + this->clients[i].dir + "").c_str() ;
                        }

                        else if (command == "mkd"){
                            if (args.size() != 1)
                                response = "501: Syntax error in parameters or arguments.";
                            else{
                                string dir_path = args[0];
                                if(mkdir((this->clients[i].dir + dir_path).c_str(),0777) == 0){
                                    if(this->clients[i].dir.back() != '/')
                                        this->clients[i].dir += '/';
                                    this->clients[i].dir += dir_path;
                                    response = ("257: " + this->clients[i].dir + " created.").c_str();
                                    this->WriteInFile(this->clients[i].user, "Make directory", dir_path);
                                }
                                else response = "500: Error";
                            }
                        }

                        else if (command == "dele"){
                            if (args.size() != 2)
                                response = "501: Syntax error in parameters or arguments.";
                            else{
                                string dele_mode = args[0];
                                if (dele_mode == "-d"){
                                    string dir_path= args[1];
                                    if(rmdir(dir_path.c_str()) == 0)
                                        response = ("250: " + dir_path + " deleted.").c_str();
                                        this->WriteInFile(this->clients[i].user, "Delete directory", dir_path);
                                }

                                //Have a problem
                                else if (dele_mode == "-f"){
                                    string filename= args[1];
                                    if(remove(filename.c_str()) == 0 )
                                        response = ("250: " + filename + " deleted.").c_str();
                                        this->WriteInFile(this->clients[i].user, "Delete file", filename);
                                }
                            }
                        }

                        else if (command == "cwd"){
                            if (args.size() != 1){
                                this->clients[i].dir = "./";
                                response = "250: Sucessful change.";
                            }
                            else if (args.size() == 1){
                                int flag = 0;
                                string dir_path = args[0];
                                if(dir_path == ".."){
                                    while(this->clients[i].dir.back() != '/')
                                        this->clients[i].dir = this->clients[i].dir.substr(0, this->clients[i].dir.size()-1);
                                    flag = 1;
                                }
                                else{
                                    string lastDir = "";
                                    string temp = this->clients[i].dir + dir_path;
                                    while(temp.back()!='/'){
                                        lastDir = temp.back() + lastDir;
                                        temp = temp.substr(0, temp.size()-1);
                                    }
                                    vector<string> lists = this->validDir(temp);
                                    for(int m=0; m<lists.size(); m++){
                                        if(lists[m]==lastDir){
                                            this->clients[i].dir += dir_path;
                                            flag = 1;
                                            break;
                                        }
                                    }
                                    
                                }
                                if (flag)
                                    response = "250: Sucessful change.";
                                else
                                    response = "500: Error (No such file or directory.)";
                            }
                            else response = "501: Syntax error in parameters or arguments.";
                        }
                        
                        //Have a problem
                        else if (command == "rename"){
                            if (args.size() != 2)
                                response = "501: Syntax error in parameters or arguments.";
                            else{
                                string from = args[0];
                                string to = args[1];
                                if (rename(from.c_str(), to.c_str()) == 0)
		                            response = "250: Sucessful change.";
                                    this->WriteInFile(this->clients[i].user, "Rename file", from, to);
                            }
                        }

                        //??????????????
                        else if (command == "ls"){
                            vector<string> lists = this->validDir(this->clients[i].dir.c_str());
                            
                            response = "226: List transfer done.";
                        }

                        else response = "501: Syntax error in parameters or arguments.";
                        
                    
                    }
                    else
                        response = "332: Need acount for login.";
                    //send responce to client
                    
                    char* message = &response[0];
                    if(send(sd, message, strlen(message), 0) != strlen(message))
                        cerr << ("send() sent a different number of bytes than expected");
                }
            }
        }


		//if ((valread = recv(client_Socket, buffer, BUFSIZE, 0)) < 0)
		//	DieWithError("recv() failed");
		/* Send received string and receive again until end of transmission */
		//while (valread > 0) { /* zero indicates end of transmission */
		//	if ((valread = recv(client_socket, buffer, BUFSIZE, 0)) < 0)
		//		DieWithError("recv() failed");
		/*for(i=0; i<MAXCLIENTS; i++) {
			cout << FD_ISSET(this->clients[i].socket, &this->readfds) << " ";
		}
		cout << endl;*/
	}
}


void server::WriteInFile(string user, string action, string input1, string input2){
    this->outfile.open("log.txt", ios::app);
    time_t now = time(0);
    char* date_time = ctime(&now);
    this->outfile << user << "  " << action << "  " << input1 << "  " << input2 << "  " << date_time;
    this->outfile.close();
}

vector<string> server::validDir(string currDir){
    struct dirent *entry;
    vector <string> lists;
    DIR *dir = opendir(currDir.c_str());

    if (dir == NULL) {
        return lists;
    }
    while ((entry = readdir(dir)) != NULL) {
        lists.push_back(entry->d_name);
    }
    closedir(dir);

    return lists;
}
#include "server.h"

int main(int argc, char** argv) {
    ifstream ifs("config.json");
    Json::Reader reader;
    Json::Value config;
    reader.parse(ifs, config);

    unsigned short commandOffset = 1; 
    unsigned int commandChannelPort = config["commandChannelPort"].asInt();
    unsigned int dataChannelPort = config["dataChannelPort"].asInt();
    string dir = "./"; 

    server* myServer = new server(commandChannelPort, dataChannelPort, dir, config);

	myServer->Run();
	

    delete myServer;

    return (EXIT_SUCCESS);
}

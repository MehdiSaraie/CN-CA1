#include "ftpserver.h"

int main(int argc, char** argv) {
    ifstream ifs("config.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);

    unsigned short commandOffset = 1; 
    unsigned int commandChannelPort = obj["commandChannelPort"].asInt();
    unsigned int dataChannelPort = obj["dataChannelPort"].asInt();
    string dir = "./"; 

    server* myServer = new server(commandChannelPort, dataChannelPort, dir, commandOffset);

	myServer->Run();
	

    delete myServer;

    return (EXIT_SUCCESS);
}

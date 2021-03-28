#include "ftpserver.h"


int main(int argc, char** argv) {
    ifstream ifs("config.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    // cout << "commandChannelPort: "<< obj["commandChannelPort"] << endl;
    // cout << "dataChannelPort: " << obj["dataChannelPort"] << endl;

    unsigned short commandOffset = 1; 
    unsigned int commandChannelPort = obj["commandChannelPort"].asInt();
    unsigned int dataChannelPort = obj["dataChannelPort"].asInt();
    string dir = "./"; 

    server* myServer = new server(commandChannelPort, dataChannelPort, dir, commandOffset);

    delete myServer;

    return (EXIT_SUCCESS);
}

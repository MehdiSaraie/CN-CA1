#include "ftpserver.h"

int main(int argc, char** argv) {
    ifstream ifs("config.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);

    unsigned int commandChannelPort = obj["commandChannelPort"].asInt();
    unsigned int dataChannelPort = obj["dataChannelPort"].asInt();

    string dir = "./"; 

    client* myClient = new client(commandChannelPort, dataChannelPort, dir);

	myClient->Communicate();

    // delete myClient;

    return (EXIT_SUCCESS);
}

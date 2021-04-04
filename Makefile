all: server client

server: ftpserver.cpp server.h server.cpp
	g++ -o server server.h server.cpp ftpserver.cpp -ljsoncpp

client: ftpclient.cpp client.h client.cpp
	g++ -o client client.h client.cpp ftpclient.cpp -ljsoncpp

.phony: clean
clean:
	rm server client log.txt
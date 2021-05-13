#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class Client{

public:
    Client();
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsResult;
    static SOCKET ourSocket;
    sockaddr_in hint;
    std::string ipAddress = "127.0.0.1";
    int port = 2000;

    void ConnectToServer();
};


class ServerListener{

public:
    ServerListener();

};

#endif // CLIENT_H

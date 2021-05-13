#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>



class ServerListener{

public:
    ServerListener();

    void Run();

};




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
    static int isConnected;
    ServerListener *serverListener;
    void ConnectToServer();
};



#endif // CLIENT_H

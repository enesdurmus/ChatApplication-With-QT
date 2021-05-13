#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>

class Client{
public:
    Client(SOCKET socket);
    SOCKET socket;
};


class NewClientListener{
public:
    NewClientListener();

    void Run();
};


class Server{
public:
    Server();
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsResult;
    static SOCKET listeningSocket;

    NewClientListener *runThread;
    //static Client clients[];
    static int isServerRunning;
    void Start();
};


class ClientListener{
public:
    ClientListener();

};




#endif // SERVER_H

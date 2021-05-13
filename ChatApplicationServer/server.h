#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>

class Client{
public:
    Client(SOCKET socket);
    ~Client();

    SOCKET socket;
};


class NewClientListener{
public:
    NewClientListener();
    ~NewClientListener();

    void Run();
};


class Server{
public:
    Server();
    ~Server();

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
    ~ClientListener();

};




#endif // SERVER_H

#include "server.h"
#include "thread"
#include <iostream>

using namespace std;

int Server::isServerRunning = 0;
SOCKET Server::listeningSocket;

Server::Server(){
    // init winsock
    wsResult = WSAStartup(ver, &wsData);
    if(wsResult != 0){
        cout << "Initialize Winsock problem! Quitting..." << endl;
        return;
    }
    cout << "Init winsock..." << endl;

    //create a socket
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listeningSocket == INVALID_SOCKET){
        cout << "Create Socket problem! Quitting..." << endl;
        return;
    }
    cout << "Socket Created..." << endl;

    //bind the socket to an ip address and port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(2000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listeningSocket, (sockaddr*)&hint, sizeof (hint));
}

Server::~Server(){

}

void Server::Start(){
    listen(listeningSocket, SOMAXCONN);

    isServerRunning = 1;

    runThread = new NewClientListener();
    thread newThread(&NewClientListener::Run, runThread);
    cout << newThread.get_id() << endl;
    newThread.join();
}

NewClientListener::NewClientListener(){

}

NewClientListener::~NewClientListener(){

}



void NewClientListener::Run(){
    while(Server::isServerRunning == 1){
        cout << "Waiting for client..." << endl;
        sockaddr_in newClient;
        int newClientSize = sizeof (newClient);
        SOCKET clientSocket = accept(Server::listeningSocket, (sockaddr*)&newClient, &newClientSize);
        Client *nClient = new Client(clientSocket);
     /*   int sendResult = send(clientSocket, s.c_str(), 4096, 0);
        if(sendResult == SOCKET_ERROR){
            cout << "gitti" << WSAGetLastError() << endl;
        }*/
        char buff[4096];
        int bytesReceived = recv(clientSocket, buff, 4096, 0);
        if(bytesReceived != SOCKET_ERROR){
            cout << string(buff, 0, bytesReceived) << endl;
        }
        string s = "clienta  gonder";
        int sendResult = send(Server::listeningSocket, s.c_str(), s.size() + 1, 0);
        if(sendResult != SOCKET_ERROR){
            cout << "gitti" << endl;
        }
    }
}

Client::Client(SOCKET socket){
    this->socket = socket;
}



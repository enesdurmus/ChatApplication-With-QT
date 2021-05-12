#include "server.h"
#include "thread"
#include <iostream>

using namespace std;

int Server::isServerRunning = 0;
SOCKET Server::listeningSocket;


Server::Server(){
    // init winsock
    wsOk = WSAStartup(ver, &wsData);
    if(wsOk != 0){
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

    cout << "Bind..." <<endl;
}


void Server::Start(){
    cout << "aloo" << endl;
    listen(listeningSocket, SOMAXCONN);

    isServerRunning = 1;

    runThread = new NewClientListener();
    thread t(&NewClientListener::Run, runThread);
    cout<<t.get_id() << endl;
    t.join();
}

NewClientListener::NewClientListener(){

}

void NewClientListener::Run(){
    while(Server::isServerRunning == 1){
        cout << "Waiting for client..." << endl;
        sockaddr_in newClient;
        int newClientSize = sizeof (newClient);
        SOCKET clientSocket = accept(Server::listeningSocket, (sockaddr*)&newClient, &newClientSize);
        Client *nClient = new Client(clientSocket);
    }
}

Client::Client(SOCKET socket){
    this->socket = socket;
    cout << "naber amk" << endl;
}



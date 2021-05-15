#include "Server.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QtDebug>
//------------------------------------------Server------------------------------------------

int Server::idCounter = 0;

QTcpSocket* Server::socket;
Server::Server (QObject * parent) : QTcpServer(parent){
    this->socket = nullptr;
    connect(this, &Server::newConnection, [&]() {socket = nextPendingConnection();});
}

void Server::incomingConnection(qintptr socketDescriptor){
    qDebug() << Server::idCounter++ << endl;
   /* QTcpSocket *cSocket = new QTcpSocket();
    cSocket->setSocketDescriptor(socketDescriptor);
    Client *nClient = new Client(cSocket);*/
}

void Server::Send(const QString &msg){
    if(Server::socket){
        QTextStream T(Server::socket);
        T << msg;
        Server::socket->flush();
    }
}

//------------------------------------------Client------------------------------------------
Client::Client(QTcpSocket *socket){
    this->socket = socket;
    this->id = Server::idCounter++;
}



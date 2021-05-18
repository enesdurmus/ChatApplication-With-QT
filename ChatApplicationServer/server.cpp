#include "Server.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QtDebug>
#include <QFile>

//------------------------------------------Server------------------------------------------

int Server::idCounter = 0;
QList<Client*> *Server::clients;
QTcpSocket* Server::socket;

Server::Server (QObject * parent) : QTcpServer(parent){
    this->socket = nullptr;
    clients = new QList<Client*>;
    connect(this, &Server::newConnection, [&]() {socket = nextPendingConnection();});
}

void Server::incomingConnection(qintptr socketDescriptor){
    Client *c = new Client(socketDescriptor);
    Server::clients->append(c);
}

void Server::Send(Client *c, const QString &msg){
    qDebug() << c->socket ->socketDescriptor() << endl;

    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << QString("the answer is");   // serialize a string

    /*
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << msg;
    c->socket->write(block);
    c->socket->flush();*/
}

//------------------------------------------Client------------------------------------------

Client::Client(qintptr socketDescriptor){
    qDebug() << this->currentThread() << endl;
    this->socketDescriptor = socketDescriptor;
    this->start();

    qDebug() << "New Client Created..." << endl;
}

Client::~Client(){
    this->destroyed();
}

void Client::run(){
    this->id = Server::idCounter++;
    this->socket = new QTcpSocket();
    if (!this->socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Error While Setting Socket " << this->socket->errorString() <<  endl;
        return;
    }

    connect(socket, &QTcpSocket::readyRead, [&](){
        QDataStream T(socket);
        in >>
    });

    this->exec();
}


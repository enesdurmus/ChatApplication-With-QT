#include "Server.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QtDebug>
#include <QMap>
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

   /* QMap<QString, QByteArray> map;
    map.insert("enes", msg);*/
    //map.insert("mal", "str");

//    QDataStream sendStream(c->socket);
    //sendStream << map;

   // qDebug() << c->name << endl;

    QFile file("C:/Users/X550V/Desktop/Proje2 (2).pdf");   //file path
    file.open(QIODevice::ReadOnly);
    QByteArray q = file.readAll();
    qDebug() << q.size() << endl;

    QMap<QString, QString> map;
    map.insert("type", "file");
    map.insert("size", QString::number(file.size()));
    QDataStream sendStream(c->socket);
    sendStream << map;

    if(map.value("type") == "file"){
        qDebug() << "girdi" << endl;
        c->socket->write(q);
    }
}

//------------------------------------------Client------------------------------------------

Client::Client(qintptr socketDescriptor){
    this->socketDescriptor = socketDescriptor;
    this->id = Server::idCounter++;
    this->socket = new QTcpSocket();
    if (!this->socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Error While Setting Socket " << this->socket->errorString() <<  endl;
        return;
    }

    this->start();
    qDebug() << "New Client Created..." << endl;
}

Client::~Client(){
    this->destroyed();
}

void Client::run(){
    connect(socket, &QTcpSocket::readyRead, [&](){
        QDataStream readStream(socket);
        QMap<QString, QString> map;
        readStream >> map;

        if(map.keys().at(0) == "connect"){
            this->name = map.value("connect");
        }
    });

    this->exec();
}


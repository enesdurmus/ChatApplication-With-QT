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

void Server::Send(Client *c, const QMap<QString, QString> &msg){
    qDebug() << c->socket ->socketDescriptor() << endl;

   /* QFile file("C:/Users/X550V/Desktop/Proje2 (2).pdf");   //file path
    file.open(QIODevice::ReadOnly);
    QByteArray q = file.readAll();
    qDebug() << q.size() << endl;
*/

   // map.insert("type", "file");
   // map.insert("size", QString::number(file.size()));
    QDataStream sendStream(c->socket);
    sendStream << msg;

   /* if(map.value("type") == "file"){
        qDebug() << "girdi" << endl;
        c->socket->write(q);
    }*/
}

void Server::BroadCast(const QMap<QString, QString> &msg){
    for(int i = 0; i < clients->size(); i++){
        Send(clients->at(i), msg);
    }

    qDebug() << "Server sends message all clients..." << endl;
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
            QString user = "user";
            QMap<QString, QString> allClients;
            allClients.insert("type", "receiveAllUsers");
            for(int i = 0; i < Server::clients->size(); i++){
                allClients.insert(user.append(QString::number(i)), Server::clients->at(i)->name);
            }
            Server::BroadCast(allClients);

            qDebug() << "User " << this->name << " Has Joined" << endl;
        }
    });

    this->exec();
}


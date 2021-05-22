#include "Server.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QtDebug>
#include <QMap>

//------------------------------------------Server------------------------------------------

int Server::idCounter = 0;
QList<Client*> *Server::clients;
QList<Room*> *Server::rooms;
QTcpSocket* Server::socket;

Server::Server (QObject * parent) : QTcpServer(parent){
    this->socket = nullptr;
    this->clients = new QList<Client*>;
    this->rooms = new QList<Room*>;
    connect(this, &Server::newConnection, [&]() {socket = nextPendingConnection();});
}

Server::~Server(){
    this->socket->close();
    delete(this);
}

void Server::incomingConnection(qintptr socketDescriptor){
    Client *c = new Client(socketDescriptor);
    Server::clients->append(c);
}

void Server::Send(Client *c, const QMap<QString, QString> &msg){
    QDataStream sendStream(c->socket);
    sendStream << msg;
    c->socket->flush();
}

void Server::SendFile(Client *c, QFile *file){
    QByteArray q = file->readAll();
    qDebug() << q.size() << endl;
    c->socket->write(q);
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
    this->rooms = new QList<Room*>;
    this->privateChats = new QList<QString>;
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

        if(map.value("type") == "connect"){

            this->name = map.value("name");
            QString u = "user";
            QMap<QString, QString> allClients;
            allClients.insert("type", "allUsers");
            for(int i = 0; i < Server::clients->size(); i++){
                allClients.insert(u.append(QString::number(i)), Server::clients->at(i)->name);
            }
            Server::BroadCast(allClients); // Send all clients the all client's name.

            qDebug() << "User " << this->name << " Has Joined..." << endl;

        }else if(map.value("type") == "createRoom"){

            Room *room = new Room(map.value("name"));
            room->clients->append(this);

            Server::rooms->append(room);
            this->rooms->append(room);

            QMap<QString, QString> allRooms;
            QString r = "room";
            allRooms.insert("type", "allRooms");
            for(int i = 0; i < Server::rooms->size(); i++){
                allRooms.insert(r.append(QString::number(i)), Server::rooms->at(i)->roomName);
            }

            Server::BroadCast(allRooms);   // Send all clients the all rooms.

            qDebug() << "User " << this->name << " Has Created A Room..." << endl;

        }else if(map.value("type") == "refreshRooms"){

            QMap<QString, QString> allRooms;
            QString r = "room";
            allRooms.insert("type", "allRooms");
            for(int i = 0; i < Server::rooms->size(); i++){
                allRooms.insert(r.append(QString::number(i)), Server::rooms->at(i)->roomName);
            }

            Server::Send(this, allRooms);   // Send all rooms to the client.

            qDebug() << "User " << this->name << " Has Refreshed The Rooms..." << endl;

        }else if(map.value("type") == "joinRoom"){

            Room *r = FindRoom(map.value("roomName"));
            this->rooms->append(r);
            r->clients->append(this);

            QMap<QString, QString> roomUsers;
            QString u = "user";
            roomUsers.insert("type", "roomUsers");
            roomUsers.insert("roomName", r->roomName);

            for(int i = 0; i < r->clients->size(); i++){
                roomUsers.insert(u.append(QString::number(i)), r->clients->at(i)->name);
            }

            for(int i = 0; i < r->clients->size(); i++){
                Server::Send(r->clients->at(i), roomUsers);  // Send all room users the users name.
            }

            qDebug() << "User " << this->name << " Has Joined The Room Named " << r->roomName << endl;

        }else if(map.value("type") == "roomMessage"){

            Room *r = FindRoom(map.value("roomName"));

            QMap<QString, QString> msg;
            msg.insert("type", "roomMessage");
            msg.insert("roomName", r->roomName);
            msg.insert("userName", this->name);
            msg.insert("message", map.value("message"));

            for(int i = 0; i < r->clients->size(); i++){
                if(r->clients->at(i)->name != this->name)
                    Server::Send(r->clients->at(i), msg);
            }

            qDebug() << "User " << this->name << " Has Send A Message To The Room Named " << r->roomName << endl;

        }else if(map.value("type") == "privateChatCreate"){

            this->privateChats->append(map.value("friendUserName"));
            FindClient(map.value("friendUserName"))->privateChats->append(this->name);

            QMap<QString, QString> msg;
            msg.insert("type", "privateChatCreate");
            msg.insert("userName", this->name);

            Server::Send(FindClient(map.value("friendUserName")), msg);

            qDebug() << "User " << this->name << " Has Open A Private Chat with " << map.value("friendUserName") << endl;

        }else if(map.value("type") == "privateChatMessage"){

            QMap<QString, QString> msg;
            msg.insert("type", "privateChatMessage");
            msg.insert("userName", this->name);
            msg.insert("message", map.value("message"));
            Server::Send(FindClient(map.value("friendUserName")), msg);

            qDebug() << "User " << this->name << " Has Send A Meesage To " << map.value("friendUserName") << endl;

        }else if(map.value("type") == "downloadFile"){

            QFile file("downloadFiles/" + map.value("fileName"));
            file.open(QIODevice::ReadOnly);

            QMap<QString, QString> fileInfo;
            fileInfo.insert("type", "fileInfo");
            fileInfo.insert("fileSize", QString::number(file.size()));

            Server::Send(this, fileInfo);

            this->socket->waitForBytesWritten(2000);

            Server::SendFile(this, &file);

            qDebug() << file.size() << endl;

        }
    });

    this->exec();
}

Room* Client::FindRoom(QString roomName){
    for(int i = 0; i < Server::rooms->size(); i++){
        if(Server::rooms->at(i)->roomName == roomName){
            return Server::rooms->at(i);
        }
    }
    return nullptr;
}

Client* Client::FindClient(QString clientName){
    for(int i = 0; i < Server::clients->size(); i++){
        if(Server::clients->at(i)->name == clientName){
            return Server::clients->at(i);
        }
    }
    return nullptr;
}


//------------------------------------------Room------------------------------------------

Room::Room(QString name){
    this->roomName = name;
    this->clients = new QList<Client*>;
}




#include "server.h"
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

    this->close();
    delete socket;
    delete clients;
    delete rooms;
}

void Server::incomingConnection(qintptr socketDescriptor){
    Client *c = new Client(socketDescriptor, m);
    Server::clients->append(c);
}

void Server::Send(Client *c, const QMap<QString, QString> &msg){
    QDataStream sendStream(c->socket);
    sendStream << msg;
    c->socket->waitForBytesWritten();
    c->socket->flush();
    c->socket->waitForBytesWritten();
    c->socket->flush();
}

void Server::SendFile(Client *c, QFile *file){
    QByteArray q = file->readAll();
    c->socket->write(q);
    c->socket->waitForBytesWritten();
    c->socket->flush();
}

void Server::BroadCast(const QMap<QString, QString> &msg){
    for(int i = 0; i < clients->size(); i++){
        Send(clients->at(i), msg);
    }
}

void Server::BroadCastClientNames(){
    QMap<QString, QString> allClients;
    allClients.insert("type", "GetAllUsers");
    for(int i = 0; i < clients->size(); i++){
        QString u = "user";
        allClients.insert(u.append(QString::number(i)), clients->at(i)->name);
    }
    BroadCast(allClients);
}

void Server::DisconnectFromServer(Client *c){
    Server::clients->removeOne(c);
    QString u = "user";
    QMap<QString, QString> allClients;
    allClients.insert("type", "allUsers"); // Send users new list because a user has disconnect.
    for(int i = 0; i < Server::clients->size(); i++){
        allClients.insert(u.append(QString::number(i)), Server::clients->at(i)->name);
    }
    Server::BroadCastClientNames(); // Send all clients the all client's name.
}

void Server::CreateRoom(Client *c, QString roomName){
    Room *room = new Room(roomName);
    room->clients->append(c);

    rooms->append(room);
    c->rooms->append(room);

    QMap<QString, QString> allRooms;
    QString r = "room";
    allRooms.insert("type", "allRooms");
    for(int i = 0; i < rooms->size(); i++){
        allRooms.insert(r.append(QString::number(i)), rooms->at(i)->roomName);
    }

    BroadCast(allRooms);   // Send all clients the all rooms.
}

void Server::DisconnectRoom(Client *c, QString roomName){
    Room *r = Server::FindRoom(roomName);
    c->rooms->removeOne(r);
    r->clients->removeOne(c);

    QMap<QString, QString> roomUsers;
    QString u = "user";
    roomUsers.insert("type", "roomUsers");
    roomUsers.insert("roomName", r->roomName);

    for(int i = 0; i < r->clients->size(); i++){
        roomUsers.insert(u.append(QString::number(i)), r->clients->at(i)->name);
    }

    for(int i = 0; i < r->clients->size(); i++){
        Send(r->clients->at(i), roomUsers);  // Send all room users the users name.
    }
}

void Server::JoinRoom(Client *c, QString roomName){
    Room *r = Server::FindRoom(roomName);
    c->rooms->append(r);
    r->clients->append(c);

    QMap<QString, QString> roomUsers;
    QString u = "user";
    roomUsers.insert("type", "roomUsers");
    roomUsers.insert("roomName", r->roomName);

    for(int i = 0; i < r->clients->size(); i++){
        roomUsers.insert(u.append(QString::number(i)), r->clients->at(i)->name);
    }

    for(int i = 0; i < r->clients->size(); i++){
        Send(r->clients->at(i), roomUsers);  // Send all room users the users name.
    }
}

void Server::RefreshRooms(Client *c){
    QMap<QString, QString> allRooms;
    QString r = "room";
    allRooms.insert("type", "allRooms");
    for(int i = 0; i < Server::rooms->size(); i++){
        allRooms.insert(r.append(QString::number(i)), Server::rooms->at(i)->roomName);
    }

    Server::Send(c, allRooms);   // Send all rooms to the client.

}

void Server::DisconnectPrivateChat(Client *c, QString friendName){
    Client *cl = FindClient(friendName);
    c->privateChats->removeOne(friendName);
    cl->privateChats->removeOne(c->name);

    QMap<QString, QString> disconnectInfo;
    disconnectInfo.insert("type", "disconnectPrivateChat");
    disconnectInfo.insert("friendName", c->name);
    Server::Send(cl, disconnectInfo);

}

void Server::SelectedRoomUsers(Client *c, QString roomName){
    Room *r = FindRoom(roomName);

    QMap<QString, QString> roomUsers;
    QString u = "user";
    roomUsers.insert("type", "selectedRoomUsers");

    for(int i = 0; i < r->clients->size(); i++){
        roomUsers.insert(u.append(QString::number(i)), r->clients->at(i)->name);
    }

    Send(c, roomUsers);   // Send all room's clients to the client.
}

bool Server::CheckUserName(QString userName){
    for(int i = 0; i < clients->size(); i++){
        if(userName == clients->at(i)->name)
            return false;
    }
    return true;
}

Room* Server::FindRoom(QString roomName){
    for(int i = 0; i < rooms->size(); i++){
        if(rooms->at(i)->roomName == roomName){
            return rooms->at(i);
        }
    }
    return nullptr;
}

Client* Server::FindClient(QString clientName){
    for(int i = 0; i < Server::clients->size(); i++){
        if(Server::clients->at(i)->name == clientName){
            return Server::clients->at(i);
        }
    }
    return nullptr;
}

void Server::ReadFile(Client *c){
    QByteArray line = c->socket->readAll();

    QFile target;
    QString dir = "downloadFiles/";
    target.setFileName(dir.append(c->fileName));

    if (!target.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "Can't open file for written";
        return;
    }

    target.write(line);

    target.close();

    c->fileSize = target.size();
}

void Server::CheckDownloadingFileSize(Client *c){

    // For rooms.
    if(c->fileRoomName != "null"){
        Room *r = Server::FindRoom(c->fileRoomName);
        QString str = "File->";

        QMap<QString, QString> fileMessage;
        fileMessage.insert("type", "fileMessageRoom");
        fileMessage.insert("roomName", c->fileRoomName);
        fileMessage.insert("fileName", str.append(c->fileName));
        fileMessage.insert("userName", c->name);

        for(int i = 0; i < r->clients->size(); i++){
            if(r->clients->at(i)->name != c->name)
                Server::Send(r->clients->at(i), fileMessage);  // Send all room users the file message.
        }
    }else{// For private chats.

        Client *cl = Server::FindClient(c->fileFriendUserName);
        QString str = "File->";

        QMap<QString, QString> fileMessage;
        fileMessage.insert("type", "fileMessagePrivateChat");
        fileMessage.insert("fileName", str.append(c->fileName));
        fileMessage.insert("userName", c->name);
        fileMessage.insert("friendUserName", c->fileFriendUserName);
        Server::Send(cl, fileMessage);

    }
    c->fileRoomName = "null";
    c->fileFriendUserName = "null";
    c->fileReading = false;
    c->fileSize = 0;
    c->receivingFileSize = -1;

}

//------------------------------------------Client------------------------------------------

Client::Client(qintptr socketDescriptor, MainWindow *m){
    this->socketDescriptor = socketDescriptor;
    this->id = Server::idCounter++;
    this->rooms = new QList<Room*>;
    this->privateChats = new QList<QString>;
    this->socket = new QTcpSocket();
    this->fileName = "null";
    this->fileFriendUserName = "null";
    this->fileRoomName = "null";
    this->fileReading = false;
    this->fileSize = -1;
    this->receivingFileSize = 0;
    this->m = m;
    if (!this->socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Error While Setting Socket " << this->socket->errorString() <<  endl;
        return;
    }

    this->start();
}

Client::~Client(){
    delete rooms;
    delete privateChats;
    socket->close();
    delete socket;
    qDebug() << name << "has deleted." << endl;
}

void Client::run(){
    connect(socket, &QTcpSocket::readyRead, [&](){

        QMap<QString, QString> map;

        // If client send us file info we get into readFile and start to read file bytes.
        if(this->fileReading){
            Server::ReadFile(this);
        }else{
            QDataStream readStream(socket);
            readStream >> map;
        }

        //Check if its file fully download.
        if(this->fileSize >= this->receivingFileSize){
            Server::CheckDownloadingFileSize(this);
            m->Display("File has downloaded...");
        }

        //Evaluate what client send to server.

        if(map.value("type") == "Connect"){

            if(Server::CheckUserName(map.value("name"))){  
                this->name = map.value("name");
                Server::BroadCastClientNames(); // Send all clients the all client's name.
                m->Display("User " + this->name + " Has Joined...");
            }else{
                QMap<QString, QString> userNameTaken;
                userNameTaken.insert("type", "userNameTaken");
                Server::clients->removeLast();
                Server::Send(this, userNameTaken);
                m->Display("User name has taken...");
              //  this->~Client();  // Destruct client
            }

        }else if(map.value("type") == "disconnect"){

            Server::DisconnectFromServer(this);
            //this->~Client();  // Destruct client
            m->Display("User " + name + " has disconnected...");

        }else if(map.value("type") == "createRoom"){

            Server::CreateRoom(this, map.value("name"));
            m->Display("User " + this->name + " Has Created A Room...");

        }else if(map.value("type") == "disconnectRoom"){

            Server::DisconnectRoom(this, map.value("roomName"));
            m->Display("User " + name + " has disconnected room named " + map.value("roomName"));

        }else if(map.value("type") == "joinRoom"){

            Server::JoinRoom(this, map.value("roomName"));
            m->Display("User " + this->name + " Has Joined The Room Named " + map.value("roomName"));

        }else if(map.value("type") == "refreshRooms"){

            Server::RefreshRooms(this);
            m->Display("User " + this->name + " Has Refreshed The Rooms...");

        }else if(map.value("type") == "disconnectPrivateChat"){

            Server::DisconnectPrivateChat(this, map.value("friendName"));
            m->Display("User " + name + " has closed the private chat named " + map.value("friendName"));

        }else if(map.value("type") == "selectedRoomUsers"){

            Server::SelectedRoomUsers(this, map.value("roomName"));
            m->Display("User " + this->name + " Has Select Another Room...");

        }else if(map.value("type") == "roomMessage"){

            Room *r = Server::FindRoom(map.value("roomName"));

            QMap<QString, QString> msg;
            msg.insert("type", "roomMessage");
            msg.insert("roomName", r->roomName);
            msg.insert("userName", this->name);
            msg.insert("message", map.value("message"));

            for(int i = 0; i < r->clients->size(); i++){
                if(r->clients->at(i)->name != this->name)
                    Server::Send(r->clients->at(i), msg);
            }

            m->Display("User " + this->name + " Has Send A Message To The Room Named " + r->roomName);

        }else if(map.value("type") == "privateChatCreate"){

            this->privateChats->append(map.value("friendUserName"));
            Server::FindClient(map.value("friendUserName"))->privateChats->append(this->name);

            QMap<QString, QString> msg;
            msg.insert("type", "privateChatCreate");
            msg.insert("userName", this->name);

            Server::Send(Server::FindClient(map.value("friendUserName")), msg);

            m->Display("User " + this->name + " Has Open A Private Chat with " + map.value("friendUserName"));

        }else if(map.value("type") == "privateChatMessage"){

            QMap<QString, QString> msg;
            msg.insert("type", "privateChatMessage");
            msg.insert("userName", this->name);
            msg.insert("message", map.value("message"));
            Server::Send(Server::FindClient(map.value("friendUserName")), msg);

            m->Display("User " + this->name + " Has Send A Meesage To " + map.value("friendUserName"));

        }else if(map.value("type") == "downloadFile"){

            QFile file("downloadFiles/" + map.value("fileName"));
            file.open(QIODevice::ReadOnly);

            QMap<QString, QString> fileInfo;
            fileInfo.insert("type", "fileInfo");
            fileInfo.insert("fileSize", QString::number(file.size()));

            Server::Send(this, fileInfo);

            this->socket->waitForBytesWritten();

            Server::SendFile(this, &file);

            m->Display("File " + map.value("fileName") + " Has Sent To User " + name);

        }else if(map.value("type") == "uploadFileRoom"){

            this->fileName = map.value("fileName");
            this->fileReading = true;
            this->receivingFileSize = map.value("fileSize").toInt();
            this->fileRoomName = map.value("roomName");
            QString path = "downloadFiles/";
            if(QFile::exists(path.append(fileName))){
                QFile file(path);
                file.remove();
            }

            m->Display("User " + this->name + " Has Upload A File ");

        }else if(map.value("type") == "uploadFilePrivateChat"){

            this->fileName = map.value("fileName");
            this->fileReading = true;
            this->receivingFileSize = map.value("fileSize").toInt();
            this->fileFriendUserName = map.value("friendClientName");
            QString path = "downloadFiles/";
            if(QFile::exists(path.append(fileName))){
                QFile file(path);
                file.remove();
            }

            m->Display("User " + this->name + " Has Upload A File ");

        }
    });

    this->exec();
}

//------------------------------------------Room------------------------------------------

Room::Room(QString name){
    this->roomName = name;
    this->clients = new QList<Client*>;
}

Room::~Room(){
    delete clients;
}



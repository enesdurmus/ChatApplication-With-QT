#include <Client.h>

Client::Client(){
    this->name = "null";
    this->rooms = new QList<RoomChat*>;
    this->privateChats = new QList<PrivateChat*>;
    this->socket = nullptr;
    isUserNameUnique = false;
    this->fileName = "null";
    this->fileDirectory = "null";
    this->fileReading = false;
    this->fileSize = -1;
    this->actualFileSize = 0;
    this->downloadingChatType = "null";
    this->downloadingChat = nullptr;
}

Client::Client(QString name){
    this->name = name;
    this->rooms = new QList<RoomChat*>;
    this->privateChats = new QList<PrivateChat*>;
    this->socket = nullptr;
    isUserNameUnique = false;
    this->downloadingChatType = "null";
    this->downloadingChat = nullptr;
    this->fileName = "null";
    this->fileDirectory = "null";
    this->fileReading = false;
    this->fileSize = -1;
    this->actualFileSize = 0;
}

Client::~Client(){
    //deleting rooms
    for(int i = 0; i < rooms->size(); i++)
        delete rooms->at(i);
    delete rooms;
    this->socket->close();
}

void Client::Send(QMap<QString, QString> map){
    QDataStream sendStream(socket);
    sendStream << map;
    this->socket->waitForBytesWritten();
    this->socket->flush();
   /* QMap<QString, QString> null;
    null.insert("null", "null");
    sendStream << null;
    this->socket->flush();*/
}

void Client::SendFile(QFile *file){
    QByteArray q = file->readAll();
    qDebug() << q.size() << endl;
    this->socket->write(q);
    this->socket->waitForBytesWritten();
    this->socket->flush();
}

void Client::DisconnectRoom(RoomChat *room){
    this->rooms->removeOne(room);
}

RoomChat* Client::FindRoom(QString roomName){
    for(int i = 0; i < this->rooms->size(); i++){
        if(roomName == this->rooms->at(i)->roomName)
            return this->rooms->at(i);
    }
    return nullptr;
}

PrivateChat* Client::FindPrivateChat(QString friendName){
    for(int i = 0; i < this->privateChats->size(); i++){
        if(friendName == this->privateChats->at(i)->friendClient)
            return this->privateChats->at(i);
    }
    return nullptr;
}

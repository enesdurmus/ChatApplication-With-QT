#include <Client.h>

Client::Client(){
    this->name = "null";
    this->rooms = new QList<RoomChat*>;
    this->privateChats = new QList<PrivateChat*>;
    this->socket = nullptr;
    this->fileReading = false;
    this->fileSize = -1;
    this->receivingFileSize = 0;
}

Client::Client(QString name){
    this->name = name;
    this->rooms = new QList<RoomChat*>;
    this->privateChats = new QList<PrivateChat*>;
    this->socket = nullptr;
    this->fileReading = false;
    this->fileSize = -1;
    this->receivingFileSize = 0;
}

void Client::Send(QMap<QString, QString> map){
    QDataStream sendStream(socket);
    sendStream << map;
}

RoomChat* Client::FindRoom(QString roomName){
    for(int i = 0; i < this->rooms->size(); i++){
        if(roomName == this->rooms->at(i)->roomName)
            return this->rooms->at(i);
    }
    return nullptr;
}

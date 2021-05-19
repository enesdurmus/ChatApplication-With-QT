#include <Client.h>

Client::Client(){
    this->name = "null";
    this->room = "null";
    this->socket = nullptr;
    this->fileReading = false;
    this->fileSize = -1;
    this->receivingFileSize = 0;
}

Client::Client(QString name){
    this->name = name;
    this->room = "null";
    this->socket = nullptr;
    this->fileReading = false;
    this->fileSize = -1;
    this->receivingFileSize = 0;
}

void Client::Send(QMap<QString, QString> map){
    QDataStream sendStream(socket);
    sendStream << map;
}

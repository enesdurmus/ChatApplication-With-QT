#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QList>
#include "roomchat.h"
#include "privatechat.h"

class RoomChat;
class PrivateChat;

class Client{
public:
    Client();
    Client(QString);
    RoomChat *FindRoom(QString roomName);
    void Send(QMap<QString, QString>);
    QMap<QString, QString>* ReceiveFromServer();
    void ReadFile();
    QTcpSocket *socket;
    QString name;
    QList<RoomChat*> *rooms;
    QList<PrivateChat*> *privateChats;

    bool fileReading = false;
    int fileSize = 0;
    int receivingFileSize = -1;
};

#endif // CLIENT_H

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
    ~Client();
    RoomChat *FindRoom(QString roomName);
    PrivateChat *FindPrivateChat(QString friendName);
    void Send(QMap<QString, QString>);
    QMap<QString, QString>* ReceiveFromServer();
    void ReadFile();
    QTcpSocket *socket;
    QString name;
    QList<RoomChat*> *rooms;
    QList<PrivateChat*> *privateChats;

    QString fileDirectory;
    QString fileName;
    bool fileReading;
    int fileSize;
    int receivingFileSize;
};

#endif // CLIENT_H

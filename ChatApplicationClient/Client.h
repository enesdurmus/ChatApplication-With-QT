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
#include "applicationwindow.h"
#include "QtMath"

class RoomChat;
class PrivateChat;

class Client{
public:
    Client();
    Client(QString);
    ~Client();

    void Send(QMap<QString, QString>);
    void SendFile(QFile*);
    void DisconnectRoom(RoomChat*);

    RoomChat *FindRoom(QString roomName);
    PrivateChat *FindPrivateChat(QString friendName);
    QMap<QString, QString>* ReceiveFromServer();

    void *downloadingChat;
    QTcpSocket *socket;
    QString name;
    QList<RoomChat*> *rooms;
    QList<PrivateChat*> *privateChats;
    QString fileDirectory;
    QString fileName;
    QString downloadingChatType;
    bool isUserNameUnique;
    bool fileReading;
    int fileSize;
    int actualFileSize;
};

#endif // CLIENT_H

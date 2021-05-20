#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QMap>
#include <QFile>
#include <QList>

class Client{
public:
    Client();
    Client(QString);
    void Send(QMap<QString, QString>);
    QMap<QString, QString>* ReceiveFromServer();
    void ReadFile();
    QTcpSocket *socket;
    QString name;
    //QList<QString> *rooms;
  //  QList<QString> *privateChats;

    bool fileReading = false;
    int fileSize = 0;
    int receivingFileSize = -1;
};

#endif // CLIENT_H

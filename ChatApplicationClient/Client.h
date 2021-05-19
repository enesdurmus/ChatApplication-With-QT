#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>

class Client{
public:
    Client();
    Client(QString);
    void Send(QMap<QString, QString>);
    QTcpSocket *socket;
    QString name;
    QString room;

    bool fileReading = false;
    int fileSize = 0;
    int receivingFileSize = -1;
};

#endif // CLIENT_H

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>
#include <QFile>

class Client;
class Room;

class Server : public QTcpServer{

    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    ~Server();
    static void Send(Client *c, const QMap<QString, QString> &msg);
    static void SendFile(Client *c, QFile *file);
    static void BroadCast(const QMap<QString, QString> &msg);
    static QTcpSocket *socket;
    static QList<Client*> *clients;
    static QList<Room*> *rooms;
    static int idCounter;
    static QString fileDirectory;
protected:
    void incomingConnection(qintptr socketDescriptor) override;

};

class Client : public QThread{

    Q_OBJECT

public:
    Client(qintptr socketDescriptor);
    Room* FindRoom(QString roomName);
    Client* FindClient(QString clientName);

    ~Client();
    qintptr socketDescriptor;
    QTcpSocket *socket;
    int id;
    QString name = "null";
    QList<Room*> *rooms;
    QList<QString> *privateChats;
    QString fileName;
    QString fileRoomName;
    QString fileClientName;
    bool fileReading;
    int fileSize;
    int receivingFileSize;

    void run() override;

};


class Room{
public:
    Room(QString);
    QString roomName = "null";
    QList<Client*> *clients;
};


#endif // SERVER_H

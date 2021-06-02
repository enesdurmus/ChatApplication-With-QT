#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include "mainwindow.h"

class Client;
class Room;
class MainWindow;

class Server : public QTcpServer{

    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    ~Server();
    static void Send(Client *c, const QMap<QString, QString> &msg);
    static void SendFile(Client *c, QFile *file);
    static void DisconnectFromServer(Client *c);
    static void CreateRoom(Client *c, QString roomName);
    static void DisconnectRoom(Client *c, QString roomName);
    static void JoinRoom(Client *c, QString roomName);
    static void BroadCast(const QMap<QString, QString> &msg);
    static void BroadCastClientNames();
    static bool CheckUserName(QString);
    static void RefreshRooms(Client *c);
    static Room* FindRoom(QString roomName);
    static Client* FindClient(QString clientName);
    static void ReadFile(Client *c);
    static void CheckDownloadingFileSize(Client *c);
    static void DisconnectPrivateChat(Client *c, QString friendName);
    static void SelectedRoomUsers(Client *c, QString roomName);

    MainWindow *m;
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
    Client(qintptr socketDescriptor, MainWindow *m);
    ~Client();

    qintptr socketDescriptor;
    QTcpSocket *socket;
    MainWindow *m;
    int id;
    QString name = "null";
    QList<Room*> *rooms;
    QList<QString> *privateChats;
    QString fileName;
    QString fileRoomName;
    QString fileFriendUserName;
    bool fileReading;
    int fileSize;
    int receivingFileSize;

    void run() override;

};


class Room{
public:
    Room(QString);
    ~Room();
    QString roomName = "null";
    QList<Client*> *clients;
};


#endif // SERVER_H

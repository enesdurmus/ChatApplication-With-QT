#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QThread>
#include <QTcpSocket>

class Client;
class ClientListenerThread;

class Server : public QTcpServer{

    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    static void Send(Client *c, const QMap<QString, QString> &msg);
    static void BroadCast(const QMap<QString, QString> &msg);
    static QTcpSocket *socket;
    static QList<Client*> *clients;
    static int idCounter;
protected:
    void incomingConnection(qintptr socketDescriptor) override;

};

class Client : public QThread{

    Q_OBJECT

public:
    Client(qintptr socketDescriptor);

    ~Client();
    qintptr socketDescriptor;
    QTcpSocket *socket;
    int id;
    QString name = "null";
    QList<QString> rooms;
    QList<QString> privateChats;
    void run() override;

};


#endif // SERVER_H

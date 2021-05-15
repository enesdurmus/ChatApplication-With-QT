#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Client;


class Server : public QTcpServer{

    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    static void Send(const QString &msg);
    static QTcpSocket *socket;
    static QList<Client> clients;
    static int idCounter;
protected:
    void incomingConnection(qintptr socketDescriptor) override;

};



class Client{

public:
    Client(QTcpSocket*);
    QTcpSocket *socket;
    int id;
    QString name = "null";

};
#endif // SERVER_H

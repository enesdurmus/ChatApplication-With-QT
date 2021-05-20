#ifndef ROOMCHAT_H
#define ROOMCHAT_H

#include <QWidget>
#include "Client.h"

class Client;

namespace Ui {
class RoomChat;
}

class RoomChat : public QWidget
{
    Q_OBJECT

public:
    explicit RoomChat(QString roomName, Client *c, QWidget *parent = nullptr);
    ~RoomChat();
    void RefreshUsers();
    void ReceiveMessage(QString, QString);
    QString roomName = "null";
    Client *client;
    QList<QString> *clients;

private slots:
    void on_sendButton_clicked();

private:
    Ui::RoomChat *ui;
};

#endif // ROOMCHAT_H

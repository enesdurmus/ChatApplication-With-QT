#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include "Client.h"

class Client;

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(Client *client, QString fClient, QWidget *parent = nullptr);
    ~PrivateChat();
    void ReceiveMessage(QString);
    Client *client;
    QString friendClient;


private:
    Ui::PrivateChat *ui;
};

#endif // PRIVATECHAT_H

#ifndef ROOMCHAT_H
#define ROOMCHAT_H

#include <QWidget>
#include "Client.h"

namespace Ui {
class RoomChat;
}

class RoomChat : public QWidget
{
    Q_OBJECT

public:
    explicit RoomChat(QString roomName, QWidget *parent = nullptr);
    ~RoomChat();
    QString roomName = "null";
    QList<QString> *clients;

private:
    Ui::RoomChat *ui;
};

#endif // ROOMCHAT_H

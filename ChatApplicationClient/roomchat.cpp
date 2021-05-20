#include "roomchat.h"
#include "ui_roomchat.h"

RoomChat::RoomChat(QString roomName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    this->roomName = roomName;
    ui->setupUi(this);
}

RoomChat::~RoomChat()
{
    delete ui;
}

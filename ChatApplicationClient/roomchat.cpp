#include "roomchat.h"
#include "ui_roomchat.h"

RoomChat::RoomChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    ui->setupUi(this);
}

RoomChat::~RoomChat()
{
    delete ui;
}

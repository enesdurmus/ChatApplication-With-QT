#include "roomchat.h"
#include "ui_roomchat.h"

RoomChat::RoomChat(QString roomName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    this->roomName = roomName;
    this->clients = new QList<QString>;
    ui->setupUi(this);
}

RoomChat::~RoomChat()
{
    delete ui;
}

void RoomChat::RefreshUsers(){
    ui->usersListWidget->clear();
    for(int i = 0; i < this->clients->size(); i++){
        ui->usersListWidget->addItem(this->clients->at(i));
    }
}

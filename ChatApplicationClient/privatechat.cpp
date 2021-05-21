#include "privatechat.h"
#include "ui_privatechat.h"

PrivateChat::PrivateChat(Client *client, QString fClient, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    this->client = client;
    this->friendClient = fClient;
    ui->setupUi(this);
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::ReceiveMessage(QString msg){
    ui->chatListWidget->addItem(this->friendClient.append("  :  ").append(msg));
}

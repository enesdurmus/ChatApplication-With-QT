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
    QString n = this->friendClient;
    ui->chatListWidget->addItem(n.append("  :  ").append(msg));
}

void PrivateChat::on_sendButton_clicked()
{
    QMap<QString, QString> sendMessage;
    sendMessage.insert("type", "privateChatMessage");
    sendMessage.insert("friendUserName" , this->friendClient);
    sendMessage.insert("message", ui->inputTextBox->text());

    QString text = this->client->name;
    text.append("  :  ").append(ui->inputTextBox->text());

    QListWidgetItem* lwi = new QListWidgetItem(text);
    ui->chatListWidget->addItem( lwi );
    lwi->setTextAlignment(Qt::AlignRight);
    lwi->setForeground(Qt::green);
    text.clear();

    client->Send(sendMessage);
}

#include "roomchat.h"
#include "ui_roomchat.h"

RoomChat::RoomChat(QString roomName, Client *c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    this->roomName = roomName;
    this->client = c;
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

void RoomChat::on_sendButton_clicked()
{
    QMap<QString, QString> sendMessage;
    sendMessage.insert("type", "roomMessage");
    sendMessage.insert("roomName", this->roomName);
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

void RoomChat::ReceiveMessage(QString userName, QString msg){
    ui->chatListWidget->addItem(userName.append("  :  ").append(msg));
}

void RoomChat::on_downloadButton_clicked()
{
    client->fileDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    client->fileDirectory.append("/");
    client->fileName = "amkk.pdf";

    QMap<QString, QString> msg;
    msg.insert("type", "downloadFile");
    msg.insert("fileName", "Proje2.pdf");
    client->Send(msg);
}

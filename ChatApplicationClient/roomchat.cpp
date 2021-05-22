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
void RoomChat::ReceiveMessageFile(QString userName, QString msg){
    QString text = userName;
    text.append("  :  ").append(msg);

    QListWidgetItem* lwi = new QListWidgetItem(text);
    ui->chatListWidget->addItem( lwi );
    lwi->setTextAlignment(Qt::AlignRight);
    lwi->setForeground(Qt::green);
    text.clear();
}

void RoomChat::on_downloadButton_clicked()
{
    client->fileDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                              "/home",
                                                              QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks);
    client->fileDirectory.append("/");
    client->fileName = ui->chatListWidget->currentItem()->text().split("->").at(1);

    qDebug() << client->fileName << endl;
    QMap<QString, QString> msg;
    msg.insert("type", "downloadFile");
    msg.insert("fileName", client->fileName);
    client->Send(msg);
}

void RoomChat::on_sendFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select File"), "/home", tr("Files (*.png *.xpm *.jpg *.txt *.xml *.pdf)"));
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly)){
        QMap<QString, QString> msg;
        msg.insert("type", "uploadFileRoom");
        msg.insert("roomName", roomName);
        msg.insert("fileName", fileName.split("/").at(fileName.split("/").size() - 1));
        msg.insert("fileSize", QString::number(file.size()));

        client->Send(msg);

        client->socket->waitForBytesWritten();

        client->SendFile(&file);

        client->socket->waitForBytesWritten();

    }else{
        QMessageBox::critical(this, "Error", "Could Not Open The File");
    }
}

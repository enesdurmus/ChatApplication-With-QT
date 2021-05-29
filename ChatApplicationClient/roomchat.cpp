#include "roomchat.h"
#include "ui_roomchat.h"

RoomChat::RoomChat(QString roomName, Client *c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->roomName = roomName;
    this->client = c;
    this->clients = new QList<QString>;
    ui->setupUi(this);
}

RoomChat::~RoomChat()
{
    client->DisconnectRoom(this);
    QMap<QString, QString> disconnectMessage;
    disconnectMessage.insert("type", "disconnectRoom");
    disconnectMessage.insert("roomName", roomName);
    client->Send(disconnectMessage);
    client->socket->waitForBytesWritten(2000);
    delete ui;
    delete clients;
    qDebug() << "Deleting room..." << endl;
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
    lwi->setBackgroundColor(Qt::transparent);
    lwi->setForeground(Qt::green);
    text.clear();
    ui->inputTextBox->clear();

    client->Send(sendMessage);
}

void RoomChat::ReceiveMessage(QString userName, QString msg){
    ui->chatListWidget->addItem(userName.append("  :  ").append(msg));
}

void RoomChat::on_downloadButton_clicked()
{
    if(ui->chatListWidget->currentItem() == NULL){
        if(ui->chatListWidget->currentItem()->text().contains("File->")){
            client->fileDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                                      QFileDialog::ShowDirsOnly
                                                                      | QFileDialog::DontResolveSymlinks);
            client->fileDirectory.append("/");
            client->fileName = ui->chatListWidget->currentItem()->text().split("->").at(1);

            qDebug() << client->fileName << endl;
            QMap<QString, QString> msg;
            msg.insert("type", "downloadFile");
            msg.insert("fileName", client->fileName);
            client->Send(msg);
        }else
            QMessageBox::critical(this, "Warning", "A File Should Be Selected");
    }else
        QMessageBox::critical(this, "Warning", "A File Should Be Selected");
}

void RoomChat::on_uploadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select File"), "/home",
                                                    tr("Files (*.png *.xpm *.jpg *.txt *.xml *.pdf)"));
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

        QString text = client->name;
        text.append("  :   File->").append(fileName.split("/").at(fileName.split("/").size() - 1));

        QListWidgetItem* lwi = new QListWidgetItem(text);
        ui->chatListWidget->addItem( lwi );
        lwi->setTextAlignment(Qt::AlignRight);
        lwi->setForeground(Qt::green);
        text.clear();

    }else{
        QMessageBox::critical(this, "Error", "Could Not Open The File");
    }
}

void RoomChat::keyPressEvent(QKeyEvent *event){
    if(event->type() == QEvent::KeyPress){
        if(event->key() == 16777220){
            on_sendButton_clicked();
        }
    }
}

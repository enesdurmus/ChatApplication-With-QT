#include "roomchat.h"
#include "ui_roomchat.h"
#include <QStandardItemModel>

RoomChat::RoomChat(QString roomName, Client *c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomChat)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->roomName = roomName;
    this->client = c;
    this->clients = new QList<QString>;
    ui->setupUi(this);
    ui->downloadProgressBar->setMaximum(100);
    UploadEmojis();
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
        QString path = "Images/";
        ui->usersListWidget->addItem(new QListWidgetItem(QIcon(path.append("user.png")), this->clients->at(i)));
    }
}

void RoomChat::on_sendButton_clicked()
{
    QMap<QString, QString> sendMessage;
    sendMessage.insert("type", "roomMessage");
    sendMessage.insert("roomName", this->roomName);
    sendMessage.insert("message", ui->inputTextBox->text());
    QListWidgetItem* lwi;

    if(ui->inputTextBox->text().contains("emo->")){
        QString path = "Images/";
        QString u = " : ";
        lwi = new QListWidgetItem(QIcon(path.append(ui->inputTextBox->text().at(5))), u.append(client->name));
    }else{
        QString text = this->client->name;
        text.append(" : ").append(ui->inputTextBox->text());
        lwi = new QListWidgetItem(text);
    }

    ui->chatListWidget->addItem( lwi );
    lwi->setTextAlignment(Qt::AlignRight);
    lwi->setBackgroundColor(Qt::transparent);
    lwi->setForeground(Qt::green);
    ui->inputTextBox->clear();
    ui->chatListWidget->scrollToBottom();
    client->Send(sendMessage);
}

void RoomChat::ReceiveMessage(QString userName, QString msg){
    if(msg.contains("emo->")){
        QString path = "Images/";
        QString u = " : ";
        ui->chatListWidget->addItem(new QListWidgetItem(QIcon(path.append(msg.at(5)).append(".png")),u.append(userName)));
    }else{
        ui->chatListWidget->addItem(userName.append("  :  ").append(msg));
    }
    ui->chatListWidget->scrollToBottom();
}

void RoomChat::on_downloadButton_clicked()
{
    if(ui->chatListWidget->currentItem() != NULL){
        if(ui->chatListWidget->currentItem()->text().contains("File->")){
            client->fileDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                                      QFileDialog::ShowDirsOnly
                                                                      | QFileDialog::DontResolveSymlinks);
            client->fileDirectory.append("/");
            client->fileName = ui->chatListWidget->currentItem()->text().split("->").at(1);
            client->downloadingChat = this;
            client->downloadingChatType = "roomChat";

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
                                                    tr("Files (*.png *.xpm *.jpg *.txt *.xml *.pdf *.rar)"));
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

void RoomChat::UpdateProgressBar(int n){
    ui->downloadProgressBar->setValue(n);
}

void RoomChat::UploadEmojis(){
    for (int i = 0; i < 10; i++) {
        QString path = "Images/";
        ui->emojiComboBox->addItem(QIcon(path.append(QString::number(i)).append(".png")), "");
    }
    ui->inputTextBox->clear();
}

void RoomChat::on_emojiComboBox_currentIndexChanged(int index)
{
    QString emojiCode = "emo->";
    emojiCode.append(QString::number(index));
    ui->inputTextBox->setText(emojiCode);
}

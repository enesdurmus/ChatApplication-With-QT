#include "privatechat.h"
#include "ui_privatechat.h"

PrivateChat::PrivateChat(Client *client, QString fClient, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->client = client;
    this->friendClient = fClient;
    ui->setupUi(this);
    ui->downloadProgressBar->setMaximum(100);
    UploadEmojis();
}

PrivateChat::~PrivateChat()
{
    client->privateChats->removeOne(this);
    QMap<QString, QString> disconnectMessage;
    disconnectMessage.insert("type", "disconnectPrivateChat");
    disconnectMessage.insert("friendName", friendClient);
    client->Send(disconnectMessage);
    client->socket->waitForBytesWritten(2000);
    delete ui;
    qDebug() << "Deleting private chat..." << endl;
}

void PrivateChat::ReceiveMessage(QString msg){
    QString n = this->friendClient;
    if(msg.contains("emo->")){
        QString path = "Images/";
        QString u = " : ";
        ui->chatListWidget->addItem(new QListWidgetItem(QIcon(path.append(msg.at(5)).append(".png")),u.append(friendClient)));
    }else{
        ui->chatListWidget->addItem(n.append(" : ").append(msg));
    }
    ui->chatListWidget->scrollToBottom();

}

void PrivateChat::on_sendButton_clicked()
{
    QMap<QString, QString> sendMessage;
    sendMessage.insert("type", "privateChatMessage");
    sendMessage.insert("friendUserName" , this->friendClient);
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

    client->Send(sendMessage);
}

void PrivateChat::on_uploadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select File"), "/home", tr("Files (*.png *.xpm *.jpg *.txt *.xml *.pdf *.rar)"));
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly)){
        QMap<QString, QString> msg;
        msg.insert("type", "uploadFilePrivateChat");
        msg.insert("friendClientName", friendClient);
        msg.insert("fileName", fileName.split("/").at(fileName.split("/").size() - 1));
        msg.insert("fileSize", QString::number(file.size()));

        client->Send(msg);

        client->socket->waitForBytesWritten();

        client->SendFile(&file);

        client->socket->waitForBytesWritten();

        QString text = friendClient;
        text.append("  :   File->").append(fileName.split("/").at(fileName.split("/").size() - 1));

        QListWidgetItem* lwi = new QListWidgetItem(text);
        ui->chatListWidget->addItem( lwi );
        lwi->setTextAlignment(Qt::AlignRight);
        lwi->setBackgroundColor(Qt::transparent);
        lwi->setForeground(Qt::green);
        text.clear();
        delete lwi;

    }else{
        QMessageBox::critical(this, "Error", "Could Not Open The File");
    }
}

void PrivateChat::on_downloadFileButton_clicked()
{
    client->fileDirectory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                              "/home",
                                                              QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks);
    client->fileDirectory.append("/");
    client->fileName = ui->chatListWidget->currentItem()->text().split("->").at(1);

    client->downloadingChat = this;
    client->downloadingChatType = "privateChat";
    QMap<QString, QString> msg;
    msg.insert("type", "downloadFile");
    msg.insert("fileName", client->fileName);
    client->Send(msg);
}

void PrivateChat::keyPressEvent(QKeyEvent *event){
    if(event->type() == QEvent::KeyPress){
        if(event->key() == 16777220){
            on_sendButton_clicked();
        }
    }
}

void PrivateChat::UpdateProgressBar(int n){
    ui->downloadProgressBar->setValue(n);
}

void PrivateChat::UploadEmojis(){
    for (int i = 0; i < 10; i++) {
        QString path = "Images/";
        ui->emojiComboBox->addItem(QIcon(path.append(QString::number(i)).append(".png")), "");
    }
    ui->inputTextBox->clear();
}

void PrivateChat::on_emojiComboBox_currentIndexChanged(int index)
{
    QString emojiCode = "emo->";
    emojiCode.append(QString::number(index));
    ui->inputTextBox->setText(emojiCode);
}

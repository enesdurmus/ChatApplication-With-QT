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

void PrivateChat::on_uploadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select File"), "/home", tr("Files (*.png *.xpm *.jpg *.txt *.xml *.pdf)"));
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
        lwi->setForeground(Qt::green);
        text.clear();

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

    qDebug() << client->fileName << endl;
    QMap<QString, QString> msg;
    msg.insert("type", "downloadFile");
    msg.insert("fileName", client->fileName);
    client->Send(msg);
}

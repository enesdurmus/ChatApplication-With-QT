#include "applicationwindow.h"
#include "ui_applicationwindow.h"

ApplicationWindow::ApplicationWindow(QString ip, int port, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationWindow)
{
    Client *theClient = new Client(name);
    theClient->socket = new QTcpSocket(this);
    theClient->socket->connectToHost("localhost", 2000);
    client = theClient;

    QMap<QString, QString> connectMap;
    connectMap.insert("connect", theClient->name);
    theClient->Send(connectMap);

    ui->setupUi(this);

    connect(client->socket, &QTcpSocket::readyRead, [&](){

        QMap<QString, QString> map;

        if(this->fileReading){
            QByteArray line = client->socket->readAll();

            QFile target;
            target.setFileName("C:/Users/X550V/Desktop/aloo.pdf");


            if (!target.open(QIODevice::WriteOnly | QIODevice::Append)) {
                qDebug() << "Can't open file for written";
                return;
            }

            target.write(line);

            target.close();

            this->fileSize = target.size();

        }else{
            QDataStream readStream(client->socket);
            readStream >> map;
            qDebug() << map.keys().at(1) << endl;

        }

        if(this->fileSize == this->receivingFileSize){
            fileReading = false;
            qDebug() << "File has downloaded..." << endl;
        }

        if(map.value("type") == "file"){
            this->fileReading = true;
            this->receivingFileSize = map.value("size").toInt();
            qDebug() << this->receivingFileSize << endl;
            qDebug() << "Receiving a file from server..." << endl;
        }
        else if(map.value("type") == "receiveAllUsers"){
            ui->usersListWidget->clear();
            qDebug() << map.size() << endl;
            QList<QString> users;
            users = map.values();
            for(int i = 1; i < map.size(); i++){
                ui->usersListWidget->addItem(users.at(i));
            }
            qDebug() << "Receiving all users from server..." << endl;
        }
    });
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}


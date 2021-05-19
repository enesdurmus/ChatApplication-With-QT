#include "applicationwindow.h"
#include "ui_applicationwindow.h"
#include <QFile>

ApplicationWindow::ApplicationWindow(QString ip, int port, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationWindow)
{
    socket = new QTcpSocket(this);
    this->name = name;
    socket->connectToHost("localhost", 2000);

    //QMap<QString, QString> connectMap;
    // connectMap.insert("connect", this->name);
    //Send(connectMap);

    ui->setupUi(this);

    connect(socket, &QTcpSocket::readyRead, [&](){
        /*QDataStream readStream(socket);
        QMap<QString, QString> map;
        readStream >> map;
        qDebug() << map.keys().at(0) << endl;
        ui->listWidget->addItem(map.keys().at(0));*/
        QMap<QString, QString> map;

        if(this->fileReading){
            QByteArray line = socket->readAll();

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
            QDataStream readStream(socket);
            readStream >> map;
            qDebug() << "laaa" << endl;
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
    });
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}

void ApplicationWindow::Send(QMap<QString, QString> map){
    QDataStream sendStream(socket);
    sendStream << map;
}

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
    connectMap.insert("type", "connect");
    connectMap.insert("name", theClient->name);
    theClient->Send(connectMap);

    ui->setupUi(this);

    connect(client->socket, &QTcpSocket::readyRead, [&](){

        QMap<QString, QString> map;

        if(client->fileReading){
            QByteArray line = client->socket->readAll();

            QFile target;
            target.setFileName("C:/Users/X550V/Desktop/aloo.pdf");


            if (!target.open(QIODevice::WriteOnly | QIODevice::Append)) {
                qDebug() << "Can't open file for written";
                return;
            }

            target.write(line);

            target.close();

            client->fileSize = target.size();

        }else{
            QDataStream readStream(client->socket);
            readStream >> map;
            qDebug() << map.keys().at(1) << endl;

        }

        if(client->fileSize == client->receivingFileSize){
            client->fileReading = false;
            qDebug() << "File has downloaded..." << endl;
        }

        // Evalute the receiving type

        if(map.value("type") == "file"){
            client->fileReading = true;
            client->receivingFileSize = map.value("size").toInt();
            qDebug() << client->receivingFileSize << endl;
            qDebug() << "Receiving a file from server..." << endl;
        }
        else if(map.value("type") == "AllUsers"){
            ui->usersListWidget->clear();
            QList<QString> users;
            users = map.values();
            for(int i = 0; i < users.size(); i++){
                if(users.at(i) != "AllUsers"){
                    ui->usersListWidget->addItem(users.at(i));}
            }
            qDebug() << "Receiving all users from server..." << endl;
        }
        else if(map.value("type") == "AllRooms"){
            ui->roomsListWidget->clear();
            QList<QString> rooms;
            rooms = map.values();
            for(int i = 0; i < rooms.size(); i++){
                if(rooms.at(i) != "AllRooms"){
                    ui->roomsListWidget->addItem(rooms.at(i));}
            }
            qDebug() << "Receiving all rooms from server..." << endl;

        }
    });
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}


void ApplicationWindow::on_createRoomButton_clicked()
{
    RoomChat *room = new RoomChat(ui->roomNameTextBox->text());
    room->setWindowTitle(room->roomName);
    client->rooms->append(room->roomName);

    QMap<QString, QString> createRoom;
    createRoom.insert("type", "createRoom");
    createRoom.insert("name", room->roomName);
    client->Send(createRoom);

    room->show();
}

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
        }else if(map.value("type") == "allUsers"){
            ui->usersListWidget->clear();
            QList<QString> users;
            users = map.values();
            for(int i = 0; i < users.size(); i++){
                if(users.at(i) != "allUsers"){
                    ui->usersListWidget->addItem(users.at(i));}
            }
            qDebug() << "Receiving all users from server..." << endl;
        }else if(map.value("type") == "allRooms"){
            ui->roomsListWidget->clear();
            QList<QString> rooms;
            rooms = map.values();
            for(int i = 0; i < rooms.size(); i++){
                if(rooms.at(i) != "allRooms"){
                    ui->roomsListWidget->addItem(rooms.at(i));}
            }
            qDebug() << "Receiving all rooms from server..." << endl;
        }else if(map.value("type") == "roomUsers"){
            RoomChat *r = client->FindRoom(map.value("roomName"));
            QList<QString> roomUsers;
            roomUsers = map.values();

            r->clients->clear();

            for (int i = 0; i < roomUsers.size(); i++) {
                if(roomUsers.at(i) != "roomUsers" && roomUsers.at(i) != r->roomName)
                    r->clients->append(roomUsers.at(i));
            }

            r->RefreshUsers();
            qDebug() << "Receiving room users from server..." << endl;
        }else if(map.value("type") == "roomMessage"){
            RoomChat *r = client->FindRoom(map.value("roomName"));

            r->ReceiveMessage(map.value("userName"), map.value("message"));

            qDebug() << "Receiving room message from server..." << endl;
        }else if(map.value("type") == "privateChatCreate"){
            PrivateChat *privateChat = new PrivateChat(this->client, map.value("userName"));
            privateChat->setWindowTitle(map.value("userName"));

            client->privateChats->append(privateChat);

            privateChat->show();

            qDebug() << "Receiving private chat info from server..." << endl;
        }else if(map.value("type") == "privateChatMessage"){
            PrivateChat *p = client->FindPrivateChat(map.value("userName"));
            p->ReceiveMessage(map.value("message"));

            qDebug() << "Receiving private chat message from server..." << endl;
        }
    });
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}

void ApplicationWindow::on_createRoomButton_clicked()
{
    RoomChat *room = new RoomChat(ui->roomNameTextBox->text(), this->client);
    room->clients->append(client->name);
    room->setWindowTitle(room->roomName);
    client->rooms->append(room);
    room->RefreshUsers();

    QMap<QString, QString> createRoom;
    createRoom.insert("type", "createRoom");
    createRoom.insert("name", room->roomName);
    client->Send(createRoom);

    room->show();
}

void ApplicationWindow::on_joinRoomButton_clicked()
{
    RoomChat *room = new RoomChat(ui->roomsListWidget->currentItem()->text(), this->client);
    room->setWindowTitle(room->roomName);
    client->rooms->append(room);

    QMap<QString, QString> joinRoom;
    joinRoom.insert("type", "joinRoom");
    joinRoom.insert("roomName", room->roomName);
    client->Send(joinRoom);

    room->show();
}

void ApplicationWindow::on_refreshRoomsButton_clicked()
{
    QMap<QString, QString> refreshRooms;
    refreshRooms.insert("type", "refreshRooms");
    client->Send(refreshRooms);
}

void ApplicationWindow::on_privateChatButton_clicked()
{
    PrivateChat *privateChat = new PrivateChat(this->client, ui->usersListWidget->currentItem()->text());
    privateChat->setWindowTitle(ui->usersListWidget->currentItem()->text());

    client->privateChats->append(privateChat);

    QMap<QString, QString> pChat;
    pChat.insert("type", "privateChatCreate");
    pChat.insert("friendUserName", ui->usersListWidget->currentItem()->text());
    client->Send(pChat);

    privateChat->show();
}

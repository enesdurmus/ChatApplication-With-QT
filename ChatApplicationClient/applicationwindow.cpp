#include "applicationwindow.h"
#include "ui_applicationwindow.h"

ApplicationWindow::ApplicationWindow(QString ip, int port, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationWindow)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

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
            QString dir = client->fileDirectory;
            target.setFileName(dir.append(client->fileName));

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
        }

        if(client->fileSize == client->receivingFileSize){
            client->fileReading = false;
            client->fileSize = 0;
            client->receivingFileSize = -1;
            qDebug() << "File has downloaded..." << endl;
        }

        // Evalute the receiving type

        if(map.value("type") == "allUsers"){

            ui->usersListWidget->clear();
            QList<QString> users;
            users = map.values();
            for(int i = 0; i < users.size(); i++){
                if(users.at(i) != "allUsers"){
                    ui->usersListWidget->addItem(users.at(i));}
            }
            client->isUserNameUnique = true;
            qDebug() << "Receiving all users from server..." << endl;

        }else if(map.value("type") == "userNameTaken"){

            client->isUserNameUnique = false;
            qDebug() << "User name has taken..." << endl;

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

        }else if(map.value("type") == "fileInfo"){

            client->fileReading = true;
            client->receivingFileSize = map.value("fileSize").toInt();
            qDebug() << "Receiving file info from server..." << endl;

        }else if(map.value("type") == "fileMessageRoom"){

            RoomChat *r = client->FindRoom(map.value("roomName"));
            //if(map.value("userName") != client->name){
            r->ReceiveMessage(map.value("userName"), map.value("fileName"));
            //}else{
            //     r->ReceiveMessageFile(map.value("userName"), map.value("fileName"));
            // }

            qDebug() << "Receiving room file message from server..." << endl;

        }else if(map.value("type") == "fileMessagePrivateChat"){

            PrivateChat *pc = client->FindPrivateChat(map.value("userName"));  //For receiver
            pc->ReceiveMessage(map.value("fileName"));

            qDebug() << "Receiving private chat file message from server..." << endl;

        }

    });
}

ApplicationWindow::~ApplicationWindow()
{
    QMap<QString, QString> disconnectMessage;
    disconnectMessage.insert("type", "disconnect");
    client->Send(disconnectMessage);
    delete client;
    delete ui;
    qDebug() << "Application window deleted..." << endl;
}

void ApplicationWindow::on_createRoomButton_clicked()
{
    bool canCreate = true;

    for(int i = 0; i < ui->roomsListWidget->count(); i++){
        if(ui->roomNameTextBox->text() == ui->roomsListWidget->takeItem(i)->text())
            canCreate = false;
    }
    if(canCreate){
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
    }else{
        QMessageBox::critical(this, "Warning", "This room name has already taken...");
    }
}

void ApplicationWindow::on_joinRoomButton_clicked()
{
    bool canJoin = true;
    for(int i = 0; i < client->rooms->size(); i++){
        if(client->rooms->at(i)->roomName == ui->roomsListWidget->currentItem()->text())
            canJoin = false;
    }

    if(canJoin){
        RoomChat *room = new RoomChat(ui->roomsListWidget->currentItem()->text(), this->client);
        room->setWindowTitle(room->roomName);
        client->rooms->append(room);

        QMap<QString, QString> joinRoom;
        joinRoom.insert("type", "joinRoom");
        joinRoom.insert("roomName", room->roomName);
        client->Send(joinRoom);

        room->show();
    }else{
        QMessageBox::critical(this, "Warning", "You have already join this room...");
    }
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

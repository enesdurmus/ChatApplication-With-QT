#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <QMap>
#include "Client.h"
#include "roomchat.h"
#include "privatechat.h"
#include <QListWidgetItem>

namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QString, int, QString, QWidget *parent = nullptr);
    ~ApplicationWindow();
    Client *client;
    void ReadFile();
    void CheckDownloadingFileSize();
    void GetUsers(QMap<QString, QString> map);
    void GetRooms(QMap<QString, QString> map);
    void GetRoomUsers(QMap<QString, QString> map);
    void GetSelectedRoomUsers(QMap<QString, QString> map);
    void CreatePrivateChat(QMap<QString, QString> map);
    void DisconnectPrivateChat(QMap<QString, QString> map);


private slots:

    void on_createRoomButton_clicked();

    void on_joinRoomButton_clicked();

    void on_refreshRoomsButton_clicked();

    void on_privateChatButton_clicked();

    void on_roomsListWidget_itemPressed(QListWidgetItem *item);

private:
    Ui::ApplicationWindow *ui;
};

#endif // APPLICATIONWINDOW_H

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <QMap>
#include "Client.h"
#include "roomchat.h"
#include "privatechat.h"

namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QString, int, QString, QWidget *parent = nullptr);
    ~ApplicationWindow();
    void ReadServer();
    Client *client;

private slots:

    void on_createRoomButton_clicked();

    void on_joinRoomButton_clicked();

    void on_refreshRoomsButton_clicked();

    void on_privateChatButton_clicked();

private:
    Ui::ApplicationWindow *ui;
};

#endif // APPLICATIONWINDOW_H

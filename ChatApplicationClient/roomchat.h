#ifndef ROOMCHAT_H
#define ROOMCHAT_H

#include <QWidget>
#include "Client.h"
#include <QFileDialog>
#include <QMessageBox>

class Client;

namespace Ui {
class RoomChat;
}

class RoomChat : public QWidget
{
    Q_OBJECT

public:
    explicit RoomChat(QString roomName, Client *c, QWidget *parent = nullptr);
    ~RoomChat();
    void RefreshUsers();
    void ReceiveMessage(QString, QString);
    QString roomName = "null";
    Client *client;
    QList<QString> *clients;

    void UpdateProgressBar(int n);
    void keyPressEvent(QKeyEvent *event) override;
    void UploadEmojis();

private slots:
    void on_sendButton_clicked();

    void on_downloadButton_clicked();

    void on_uploadFileButton_clicked();

    void on_emojiComboBox_currentIndexChanged(int index);

private:
    Ui::RoomChat *ui;
};

#endif // ROOMCHAT_H

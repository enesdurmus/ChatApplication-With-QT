#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <QMap>
#include "Client.h"

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
    //QTcpSocket *socket;

    bool fileReading = false;
    int fileSize = 0;
    int receivingFileSize = -1;

private slots:

private:
    Ui::ApplicationWindow *ui;
};

#endif // APPLICATIONWINDOW_H

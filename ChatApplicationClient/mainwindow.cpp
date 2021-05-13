#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationwindow.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bkgnd("C:/Users/X550V/Desktop/MyProjects/ChatApplicationClient/BackGround.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loginButton_clicked()
{
    ApplicationWindow *nw = new ApplicationWindow(ui->userNameTextField->toPlainText().toStdString());
    Client *client = new Client();
    client->ConnectToServer();
    std::cout << "geciyormu buraya" << endl;
    nw->show();
    this->close();
}

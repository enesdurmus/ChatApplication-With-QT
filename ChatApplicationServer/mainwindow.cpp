#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->server = new Server(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

void MainWindow::on_startServerButton_clicked()
{
    if(!server->listen(QHostAddress::Any, ui->portNumberTextBox->value())){
        QMessageBox::critical(this, "Error", server->errorString());
    }else{
        QMessageBox::information(this, "Server", "Started to listening...");
    }
}

void MainWindow::on_sendButton_clicked()
{
    QMap<QString, QString> msg;
    msg.insert("message", ui->messageTextBox->text());
    server->Send(Server::clients->at(ui->portNumberTextBox->value()) ,msg);
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

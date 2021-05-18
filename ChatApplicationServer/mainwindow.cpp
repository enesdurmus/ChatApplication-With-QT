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
    server->Send(Server::clients->at(ui->portNumberTextBox->value()) ,ui->messageTextBox->text());
    qDebug() << Server::clients->at(ui->portNumberTextBox->value())->socket->socketDescriptor() << endl;
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

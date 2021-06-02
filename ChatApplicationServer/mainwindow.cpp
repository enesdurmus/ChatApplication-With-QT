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
    this->server->m = this;
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

void MainWindow::Display(const QString msg){
    ui->listWidget->addItem(msg);
    ui->listWidget->scrollToBottom();
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

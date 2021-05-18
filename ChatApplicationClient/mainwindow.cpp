#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, [&](){
        QTextStream T(socket);
        ui->listWidget->addItem(T.readAll());
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    //socket->connectToHost(ui->serverIpTextBox->text(), ui->portTextBox->value());
    socket->connectToHost("localhost", 2000);
}

void MainWindow::on_QuitButton_clicked()
{

}

void MainWindow::on_pushButton_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << "msg";
    socket->write(block);
}

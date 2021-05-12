#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "applicationwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap bkgnd("C:/Users/X550V/Desktop/MyProjects/ChatApplication/BackGround.jpg");
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
    ApplicationWindow *nw = new ApplicationWindow(ui->userNameTextField->toPlainText());
    nw->show();
    this->close();
}

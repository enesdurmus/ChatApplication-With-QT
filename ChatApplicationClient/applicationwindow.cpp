#include "applicationwindow.h"
#include "ui_applicationwindow.h"
#include <QDebug>

ApplicationWindow::ApplicationWindow(QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationWindow)
{
    ui->setupUi(this);
    this->userName = userName;
}

ApplicationWindow::~ApplicationWindow()
{
    delete ui;
}

void ApplicationWindow::on_pushButton_clicked()
{
    qDebug() << this->userName;
}

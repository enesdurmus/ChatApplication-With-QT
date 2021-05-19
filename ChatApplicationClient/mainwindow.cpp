#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    ApplicationWindow *appW = new ApplicationWindow(ui->serverIpTextBox->text(), ui->portTextBox->value(), ui->userNameTextBox->text());
    appW->show();
    hide();
}

void MainWindow::on_QuitButton_clicked()
{
    close();
}


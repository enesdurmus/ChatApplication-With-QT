#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    ApplicationWindow *appW = new ApplicationWindow(ui->serverIpTextBox->text(), ui->portTextBox->value(), ui->userNameTextBox->text());
    if(appW->client->socket->waitForConnected(3000)){  // if its connect to server open.
        appW->show();
        hide();
    }else{  // if not show a message to user.
        QMessageBox::critical(this, "Error", "Could Not Connect To Server");
    }
}

void MainWindow::on_QuitButton_clicked()
{
    close();
}


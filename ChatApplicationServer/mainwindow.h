#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    Server *server;
    ~MainWindow();

private slots:
    void on_startServerButton_clicked();

    void on_sendButton_clicked();

    void on_quitButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <string.h>
using namespace std;

namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationWindow(string userName, QWidget *parent = nullptr);
    ~ApplicationWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ApplicationWindow *ui;
    string userName;
};

#endif // APPLICATIONWINDOW_H

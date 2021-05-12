#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <QString>
namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QString userName, QWidget *parent = nullptr);
    ~ApplicationWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ApplicationWindow *ui;
    QString userName;
};

#endif // APPLICATIONWINDOW_H

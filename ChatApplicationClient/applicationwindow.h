#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QMap>

namespace Ui {
class ApplicationWindow;
}

class ApplicationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ApplicationWindow(QString, int, QString, QWidget *parent = nullptr);
    ~ApplicationWindow();
    void Send(QMap<QString, QString>);
    QTcpSocket *socket;
    QString name;

    bool fileReading = false;
    int fileSize = 0;
    int receivingFileSize = -1;

private:
    Ui::ApplicationWindow *ui;
};

#endif // APPLICATIONWINDOW_H

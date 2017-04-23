#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QUdpSocket>
#include <QHostAddress>

#include "request.h"
#include "password.h"

struct Request;
class Password;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
        QUdpSocket *socket;
        QTextEdit *edit;
        QLineEdit *line;

        Request *request, *reply;
        Password *log_in;

        QString username, password;

private:
        void Layout();

private slots:

            void SendDatagram(int);
            void ProcessData();
            void Sending();
            void Entering(QString, QString);

signals:
            void signal_send(int);
};

#endif // WINDOW_H

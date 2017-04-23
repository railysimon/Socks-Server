#include "window.h"

Window::Window(QWidget *parent): QDialog(parent)
{
    socket = new QUdpSocket(this); // creating new socket
    socket->bind(2525); // listen 2525 port
    connect(socket, SIGNAL(readyRead()), this, SLOT(ProcessData()));

    Layout();

    request = new Request;
    reply = new Request;

    SendDatagram(1);
}

Window::~Window()
{

}

void Window::Layout() // creating UI
{
    this->setStyleSheet("background: rgb(95, 96, 97);");

    edit = new QTextEdit;
    edit->setReadOnly(true);
    edit->setStyleSheet("border: 1px solid white;"
                        "color: white; font-weight: bold;");

    line = new QLineEdit;
    line->setEnabled(false);
    connect(line, SIGNAL(returnPressed()), this, SLOT(Sending()));
    connect(this, SIGNAL(signal_send(int)), this, SLOT(SendDatagram(int)));
    line->setStyleSheet("border: 1px solid white;"
                        "color: white; font-weight: bold;");

    QLabel *title = new QLabel("<h2>Server</h2>");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(title);
    layout->addWidget(edit);
    layout->addWidget(line);
    this->setLayout(layout);
    this->setWindowTitle("Server");
}

void Window::SendDatagram(int number) // methode for sending data
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    // standart information
    request->version = 5;
    request->count_methodes = 2;
    request->methodes[0] = 0;
    request->methodes[1] = 2;
    request->port = 2323;
    request->ip = QHostAddress::LocalHost;
    request->message = line->text();

    if(number == 1) // first query
    {
       out << QString("Server: ") << request->version << request->count_methodes;
       out << request->methodes[0] << request->methodes[1];
    }

    else if(number == 2) // standart query
    {
        out << QString("Server: ") << request->version << request->count_methodes;
        out << request->methodes[0] << request->methodes[1] << request->port;
        out << request->ip << request->message;
        edit->append("Server: " + line->text());
    }
    else if(number == 3) // authorization query
    {
        out << username << password;
    }

    socket->writeDatagram(datagram, QHostAddress::LocalHost, 2424);
    line->clear();
}

void Window::ProcessData() // processing data from server
{
    QByteArray datagram;

    do
        {
            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(), datagram.size());

        } while(socket->hasPendingDatagrams()); // getting data from client

    QString id;
    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_7);

    if(datagram.size() == 20) // first answer from PROXY (SOCKSv, choosen methode)
    {
        in >> id;
        in >> reply->version >> reply->methodes[0];

        if(!reply->methodes[0]) // if methode code is 0x00 - connection without authorization
        {
            edit->append(id + "Connected...");
            line->setEnabled(true);
        }
        else            // else creating window for autorization
        {
            log_in = new Password;
            connect(log_in, SIGNAL(signal_userdata(QString,QString)), this, SLOT(Entering(QString,QString)));
            log_in->show();
        }
    }
    else
    {
        in >> id;
        if(id == "Answer: ")
        {
            in >> reply->message;
            edit->append(id + reply->message);
        }
        else if(id != "PROXY: " || id != "Client: ")
        {
            edit->append(id); // answer from server (authorized or no authorized)
            line->setEnabled(true);
        }
        else // ?
        {
            in >> reply->version >> reply->count_methodes >> reply->methodes[0];
            in >> reply->methodes[1] >> reply->port >> reply->ip >> reply->message;
        }
    }
}

void Window::Sending() // if press enter - send your message
{
    emit signal_send(2);
}

void Window::Entering(QString user, QString passwd) // slot for authorization
{
    username = user;
    password = passwd;

    SendDatagram(3);
}

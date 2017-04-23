#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QHostAddress>

struct Request
{
    quint8 version;
    quint8 count_methodes;
    quint8 methodes[2];
    quint16 port;
    QHostAddress ip;
    QString message;
};

#endif // REQUEST_H

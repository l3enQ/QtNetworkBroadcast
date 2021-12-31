#include "clienthelper.h"
#include <QTcpSocket>

ClientHelper::ClientHelper(QTcpSocket *clientSocket, QObject *parent): QObject(parent),
    socket(clientSocket)
{

    in.setDevice(clientSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(clientSocket, &QIODevice::readyRead, this, [&](){
        in.startTransaction();
        QString nextFortune;
        in >> nextFortune;
        in.commitTransaction();

        emit messageRecieved(nextFortune);
    });


    connect(socket, &QAbstractSocket::disconnected,
            socket, &QObject::deleteLater);
    connect(socket, &QAbstractSocket::disconnected,
            this,   &QObject::deleteLater);
}

QTcpSocket *ClientHelper::getSocket() const
{
    return socket;
}

void ClientHelper::sendMessage(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << message;


    socket->write(block);
}

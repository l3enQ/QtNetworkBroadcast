#ifndef CLIENTREADER_H
#define CLIENTREADER_H

#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class ClientHelper : public QObject
{
    Q_OBJECT

public:
    explicit ClientHelper(QTcpSocket *clientSocket, QObject *parent = nullptr);
    QTcpSocket *getSocket() const;

    void sendMessage(QString message);

signals:
    void messageRecieved(QString message);

private:
    QTcpSocket *socket;
    QDataStream in;
};

#endif // CLIENTREADER_H

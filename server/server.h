#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class ClientHelper;

class Server : public QObject
{
    Q_OBJECT

public:
    static Server *instance();
    void start(int port);
    void disconnectSocket(int index);

private slots:
    void sessionOpened();
    void incomingConnection();

signals:
    void started(QString ip, quint16 port);
    void error(QString error);
    void connected(QString ip);
    void messageReceived(QString source, QString message);
    void socketDisconnected(int index);

private:
    explicit Server(QObject *parent = nullptr);
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;

    QList<ClientHelper *> connectedClients;


    int _port;
};

#endif // SERVER_H

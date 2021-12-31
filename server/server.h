#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server : public QObject
{
    Q_OBJECT

public:
    static Server *instance();
    void start(int port);

private slots:
    void sessionOpened();
    void incomingConnection();

signals:
    void started(QString ip, quint16 port);
    void error(QString error);
    void connected(QString ip);

private:
    explicit Server(QObject *parent = nullptr);
    QTcpServer *tcpServer = nullptr;
    QNetworkSession *networkSession = nullptr;

    int _port;
};

#endif // SERVER_H

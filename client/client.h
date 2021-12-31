#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDataStream>

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Client : public QObject
{
    Q_OBJECT

public:
    static Client *instance();
    void connectHost(QHostAddress address, quint16 port);

private:
    explicit Client(QObject *parent = nullptr);

    QDataStream in;
    QTcpSocket *tcpSocket = nullptr;
    QNetworkSession *networkSession = nullptr;

signals:
    void connected();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetFortuneButton();
    void sessionOpened();
    void readFortune();
};

#endif // CLIENT_H

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
    void disconnectHost();
    void sendMessage(QString message);
    QString lastError();

    QStringList getHosts() {return hosts;}

private:
    explicit Client(QObject *parent = nullptr);

    QDataStream in;
    QTcpSocket *tcpSocket = nullptr;
    QNetworkSession *networkSession = nullptr;

    QStringList hosts;
    void initHosts();

signals:
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError socketError);
    void newRead(QString message);

private slots:
    void enableGetFortuneButton();
    void sessionOpened();
    void read();
};

#endif // CLIENT_H

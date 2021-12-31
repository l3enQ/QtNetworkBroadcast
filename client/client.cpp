#include <QtNetwork>
#include <QDebug>

#include "client.h"

Client *Client::instance()
{
//    static Client clientInstance;
    return new Client();//clientInstance;
}

Client::Client(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
{
    initHosts();

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(tcpSocket, &QAbstractSocket::connected,    this, &Client::connected);
    connect(tcpSocket, &QAbstractSocket::disconnected, this, &Client::disconnected);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::read);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Client::displayError);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

//        getFortuneButton->setEnabled(false);
//        statusLabel->setText(tr("Opening network session."));

        networkSession->open();
    }
}

void Client::initHosts()
{
//    QString name = QHostInfo::localHostName();
//    if (!name.isEmpty()) {
//        hosts.append(name);
//        QString domain = QHostInfo::localDomainName();
//        if (!domain.isEmpty())
//            hosts.append(name + QChar('.') + domain);
//    }

//    if (name != QLatin1String("localhost"))
//        hosts.append(QString("localhost"));

    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hosts.append(ipAddressesList.at(i).toString());
    }

    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hosts.append(ipAddressesList.at(i).toString());
    }
}

void Client::connectHost(QHostAddress address, quint16 port)
{
//    getFortuneButton->setEnabled(false);
    tcpSocket->abort();

    tcpSocket->connectToHost(address, port);
}

void Client::disconnectHost()
{
    tcpSocket->disconnectFromHost();
}

void Client::sendMessage(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << message;
    tcpSocket->write(block);
}

QString Client::lastError()
{
    return tcpSocket->errorString();
}

void Client::read()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;


    emit newRead(nextFortune);

    if (!in.commitTransaction())
        return;

//    statusLabel->setText(currentFortune);
//    getFortuneButton->setEnabled(true);
}


void Client::enableGetFortuneButton()
{
//    getFortuneButton->setEnabled((!networkSession || networkSession->isOpen()) &&
//                                 !hostCombo->currentText().isEmpty() &&
//                                 !portLineEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

//    statusLabel->setText(tr("This examples requires that you run the "
//                            "Fortune Server example as well."));

//    enableGetFortuneButton();
}

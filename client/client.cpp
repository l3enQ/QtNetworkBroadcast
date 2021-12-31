#include <QtNetwork>
#include <QDebug>

#include "client.h"

Client *Client::instance()
{
    static Client clientInstance;
    return &clientInstance;
}

Client::Client(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
{
    QStringList hosts;

    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hosts.append(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hosts.append(name + QChar('.') + domain);
    }

    if (name != QLatin1String("localhost"))
        hosts.append(QString("localhost"));

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


    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(tcpSocket, &QAbstractSocket::connected, this, &Client::connected);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFortune);
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

void Client::connectHost(QHostAddress address, quint16 port)
{
//    getFortuneButton->setEnabled(false);
    tcpSocket->abort();

    tcpSocket->connectToHost(address, port);
}

void Client::readFortune()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    if (!in.commitTransaction())
        return;

//    if (nextFortune == currentFortune) {
//        QTimer::singleShot(0, this, &Client::requestNewFortune);
//        return;
//    }

//    currentFortune = nextFortune;
//    statusLabel->setText(currentFortune);
//    getFortuneButton->setEnabled(true);
}


void Client::displayError(QAbstractSocket::SocketError socketError)
{

    qDebug() << Q_FUNC_INFO << __LINE__ << socketError;
//    switch (socketError) {
//    case QAbstractSocket::RemoteHostClosedError:
//        break;
//    case QAbstractSocket::HostNotFoundError:
//        QMessageBox::information(this, tr("Fortune Client"),
//                                 tr("The host was not found. Please check the "
//                                    "host name and port settings."));
//        break;
//    case QAbstractSocket::ConnectionRefusedError:
//        QMessageBox::information(this, tr("Fortune Client"),
//                                 tr("The connection was refused by the peer. "
//                                    "Make sure the fortune server is running, "
//                                    "and check that the host name and port "
//                                    "settings are correct."));
//        break;
//    default:
//        QMessageBox::information(this, tr("Fortune Client"),
//                                 tr("The following error occurred: %1.")
//                                 .arg(tcpSocket->errorString()));
//    }

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

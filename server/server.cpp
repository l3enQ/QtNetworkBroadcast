#include "server.h"
#include "clienthelper.h"

Server::Server(QObject *parent)
    : QObject(parent)
{
}

Server *Server::instance()
{
    static Server serverInstance;
    return &serverInstance;
}

void Server::start(int port)
{
    _port = port;

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
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        networkSession->open();

    } else {
        sessionOpened();
    }
}

void Server::stop()
{
    tcpServer->close();
    foreach (auto client, connectedClients) {
        client->getSocket()->disconnectFromHost();
    }
    emit stopped();
}

void Server::disconnectSocket(int index)
{
    if (connectedClients.count() > index)
        connectedClients.at(index)->getSocket()->disconnectFromHost();
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
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
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::incomingConnection);

    if (!tcpServer->listen(QHostAddress::Any, _port)) {
        emit error(tcpServer->errorString());
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    emit started(ipAddress, tcpServer->serverPort());
}

void Server::incomingConnection()
{
    auto clientReader = new ClientHelper(tcpServer->nextPendingConnection());

    connectedClients.append(clientReader);
    connect(clientReader, &ClientHelper::destroyed,
            this, [=](){
        int index = connectedClients.indexOf(clientReader);
        connectedClients.removeOne(clientReader);

        emit socketDisconnected(index);
    });

    connect(clientReader, &ClientHelper::messageRecieved, this, [=](QString message){
        emit messageReceived(clientReader->getSocket()->peerAddress().toString(),
                             message);

        foreach (auto client, connectedClients) {
//            if  (client == clientReader)
//                continue;

            client->sendMessage(message);
        }
    });

    emit connected(clientReader->getSocket()->peerAddress().toString());
}

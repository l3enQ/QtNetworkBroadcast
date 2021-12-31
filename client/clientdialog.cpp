#include "clientdialog.h"
#include "ui_clientdialog.h"

#include "client.h"

#include <QHostAddress>
#include <QMessageBox>

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    _client = Client::instance();

    connect(_client, &Client::connected, this, [&](){
        ui->btnConnect->setText("connected");

        ui->progressBar->hide();
    });

    connect(_client, &Client::displayError, this, [&](QAbstractSocket::SocketError socketError){
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("Client"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("Client"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the fortune server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            QMessageBox::information(this, tr("Client"),
                                     tr("The following error occurred: %1.")
                                     .arg(_client->lastError()));
        }

        disable();
    });

    connect(_client, &Client::newRead, this, [&](QString message) {

        qDebug() << Q_FUNC_INFO << __LINE__ << message;
        ui->listWidget->insertItem(0, message);
    });

    ui->cbxHosts->insertItems(0, _client->getHosts());
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::on_leMessage_returnPressed()
{
    sendMessage();
}

void ClientDialog::on_btnSend_clicked()
{
    sendMessage();
}

void ClientDialog::on_btnConnect_clicked()
{
    ui->btnConnect->setDisabled(true);
    ui->progressBar->show();

    _client->connectHost(QHostAddress(ui->leHost->text()), ui->spbxPort->value());
}

void ClientDialog::on_cbxHosts_currentIndexChanged(const QString &arg1)
{
    ui->leHost->setText(arg1);
}

void ClientDialog::on_leMessage_textEdited(const QString &arg1)
{
    ui->btnSend->setDisabled(arg1.isEmpty());
}

void ClientDialog::sendMessage()
{
    _client->sendMessage(ui->leMessage->text());
}

void ClientDialog::disable()
{

}

void ClientDialog::enable()
{

}

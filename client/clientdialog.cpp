#include "clientdialog.h"
#include "ui_clientdialog.h"

#include "client.h"

#include <QHostAddress>

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
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::on_leMessage_returnPressed()
{

}

void ClientDialog::on_btnSend_clicked()
{

}

void ClientDialog::on_btnConnect_clicked()
{
    ui->btnConnect->setDisabled(true);
    ui->progressBar->show();

    _client->connectHost(QHostAddress(ui->leHost->text()), ui->spbxPort->value());
}

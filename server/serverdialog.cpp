#include "serverdialog.h"
#include "ui_serverdialog.h"

#include "server.h"

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    _server = Server::instance();
    connect(_server, &Server::started, this, [&] (QString ipAddress, quint16 port) {
       ui->lStatus->setText(tr("The server is running on\nIP: %1\nport: %2")
                            .arg(ipAddress).arg(port));
       ui->progressBar->hide();
       ui->lStatus->show();
       ui->btnStartServer->setText("Stop");
       ui->btnStartServer->setEnabled(true);
    });

    connect(_server, &Server::error, this, [&] (QString errorStr) {
        ui->lStatus->setText(errorStr);

        ui->progressBar->hide();
        ui->lStatus->show();
        ui->btnStartServer->setText("Start");
        ui->btnStartServer->setEnabled(true);
    });

    connect(_server, &Server::connected, this, [&] (QString ip) {
        ui->textBrowser->insertPlainText(ip);
        ui->textBrowser->insertPlainText("\n");
    });
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::on_btnStartServer_clicked()
{
    ui->btnStartServer->setDisabled(true);

    ui->progressBar->show();
    ui->lStatus->hide();


    _server->start(ui->spbxPort->value());
}

#include "serverdialog.h"
#include "ui_serverdialog.h"

#include <QMenu>
#include <QCloseEvent>

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
       ui->pStart->setEnabled(true);
       ui->stackedWidget->setCurrentWidget(ui->pStop);

       ui->textBrowser->insertPlainText(QString("%0 : Server started.\n")
                                        .arg(QDateTime::currentDateTime().toString()));
    });

    connect(_server, &Server::stopped, this, [&] () {
        ui->lStatus->hide();
        ui->stackedWidget->setCurrentWidget(ui->pStart);

        ui->textBrowser->insertPlainText(QString("%0 : Server stopped.\n")
                                         .arg(QDateTime::currentDateTime().toString()));
    });

    connect(_server, &Server::error, this, [&] (QString errorStr) {
        ui->lStatus->setText(errorStr);
        ui->progressBar->hide();
        ui->lStatus->show();
        ui->pStart->setEnabled(true);
    });

    connect(_server, &Server::connected, this, [&] (QString ip) {
        ui->lwConnectedClients->addItem(ip);

        ui->textBrowser->insertPlainText(QString("%0 : %1 connected.\n")
                                         .arg(QDateTime::currentDateTime().toString(),
                                              ip));
    });

    connect(_server, &Server::messageReceived, this, [&] (QString source, QString message) {
        ui->textBrowser->insertPlainText(QString("%0 : (%2) %1\n")
                                         .arg(QDateTime::currentDateTime().toString(),
                                              message,
                                              source));
    });

    connect(_server, &Server::socketDisconnected, this, [&] (int index) {
       auto item = ui->lwConnectedClients->takeItem(index);
       ui->textBrowser->insertPlainText(QString("%0 : %1 disconnected.\n")
                                        .arg(QDateTime::currentDateTime().toString(),
                                             item->data(Qt::DisplayRole).toString()));
       delete item;
    });
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::on_btnStartServer_clicked()
{
    ui->pStart->setDisabled(true);

    ui->progressBar->show();
    ui->lStatus->hide();


    _server->start(ui->spbxPort->value());
}

void ServerDialog::on_lwConnectedClients_customContextMenuRequested(const QPoint &pos)
{
    auto rows = ui->lwConnectedClients->selectionModel()->selectedRows(0);
    if (!rows.isEmpty()) {
        QMenu m(ui->lwConnectedClients);
        m.addAction("disconnect");
        auto action = m.exec(ui->lwConnectedClients->mapToGlobal(pos));
        if (action)
            _server->disconnectSocket(rows.first().row());
    }
}

void ServerDialog::on_btnStop_clicked()
{
    _server->stop();
}

void ServerDialog::closeEvent(QCloseEvent *e)
{
    if (ui->stackedWidget->currentIndex() == 1)
        _server->stop();
    e->accept();
}

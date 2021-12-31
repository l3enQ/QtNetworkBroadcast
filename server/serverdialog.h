#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>

namespace Ui {
class ServerDialog;
}

class Server;

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = nullptr);
    ~ServerDialog();

private slots:
    void on_btnStartServer_clicked();

private:
    Ui::ServerDialog *ui;

    Server *_server;
};

#endif // SERVERDIALOG_H

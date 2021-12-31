#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class ClientDialog;
}

class Client;

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = nullptr);
    ~ClientDialog();

private slots:
    void on_leMessage_returnPressed();
    void on_btnSend_clicked();
    void on_btnConnect_clicked();
    void on_cbxHosts_currentIndexChanged(const QString &arg1);
    void on_leMessage_textEdited(const QString &arg1);
    void on_cbxHosts_editTextChanged(const QString &arg1);
    void on_btnDisconnect_clicked();

private:
    Ui::ClientDialog *ui;

    Client *_client;

    void sendMessage();
    void closeEvent(QCloseEvent *e) override;
};

#endif // CLIENTDIALOG_H

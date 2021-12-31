#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server/serverdialog.h"
#include "client/clientdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnServer_clicked()
{
    auto dialog = new ServerDialog(this);
    dialog->show();
}

void MainWindow::on_btnClient_clicked()
{
    auto dialog = new ClientDialog(this);
    dialog->show();
}

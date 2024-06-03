#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountDialog.h"
#include "LoginAccountDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->createAccountAction, &QAction::triggered, this, &MainWindow::createAccount);
    connect(ui->loginAccountAction, &QAction::triggered, this, &MainWindow::loginAccount);
}

void MainWindow::createAccount() {
    CreateAccountDialog createAccountDialog(this);
    createAccountDialog.exec();
}

void MainWindow::loginAccount() {
    LoginAccountDialog loginAccountDialog(this);
    loginAccountDialog.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}


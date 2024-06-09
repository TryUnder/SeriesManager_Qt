#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountDialog.h"
#include "LoginAccountDialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbManager("D:\\P.R.I.V\\Uczelnia\\Semestr_VIII\\Programowanie_Wieloplatformowe\\Projekt_Seriale_Db.db")
    , isLoggedIn(false)
{
    ui->setupUi(this);
    connect(ui->createAccountAction, &QAction::triggered, this, &MainWindow::createAccount);
    connect(ui->loginAccountAction, &QAction::triggered, this, &MainWindow::loginAccount);
}

void MainWindow::createAccount() {
    if (!dbManager.doesUserExist()) {
        CreateAccountDialog createAccountDialog(this);
        createAccountDialog.exec();
    } else {
        QMessageBox::information(this, "Informacja", "Konto już istnieje. Zaloguj się.");
    }
}

void MainWindow::loginAccount() {
    LoginAccountDialog loginAccountDialog(this);

    if (isLoggedIn == true) {
        return;
    }

    if (loginAccountDialog.exec() == QDialog::Accepted) {
        isLoggedIn = true;
        QMessageBox::information(this, "Sukces", "Zalogowano pomyślnie");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


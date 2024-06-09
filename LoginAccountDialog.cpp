#include "LoginAccountDialog.h"
#include "ui_loginAccount.h"
#include <QMessageBox>
#include <QSqlQuery>

LoginAccountDialog::LoginAccountDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoginAccountDialog)
{
    ui->setupUi(this);

    connect(ui->loginButton, &QPushButton::clicked, this, &LoginAccountDialog::loginAccount);
    connect(ui->loginButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginAccountDialog::loginAccount() {
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić wszystkie pola.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM user WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next() && query.value(0).toInt() == 1) {
        accept();
    } else {
        QMessageBox::warning(this, "Błąd", "Niepoprawna nazwa użytkownika lub hasło.");
    }
}

LoginAccountDialog::~LoginAccountDialog() {
    delete ui;
}

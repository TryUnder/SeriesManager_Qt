#include "LoginAccountDialog.h"
#include "ui_loginAccount.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "accountmanager.h"

LoginAccountDialog::LoginAccountDialog(DatabaseManager* dbManager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoginAccountDialog),
    m_dbManager(dbManager)
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

    accountmanager accountManager(m_dbManager);
    if (accountManager.loginAccount(username, password)) {
        accept();
    } else {
        QMessageBox::warning(this, "Błąd", "Niepoprawna nazwa użytkownika lub hasło.");
    }
}

LoginAccountDialog::~LoginAccountDialog() {
    delete ui;
}

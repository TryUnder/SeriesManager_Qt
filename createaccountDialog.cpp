#include "createaccountDialog.h"
#include "ui_createaccount.h"
#include <QSqlQuery>
#include <QMessageBox>

CreateAccountDialog::CreateAccountDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CreateAccountDialog)
{
    ui->setupUi(this);

    connect(ui->createButton, &QPushButton::clicked, this, &CreateAccountDialog::createAccount);
    connect(ui->cancelCreateButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CreateAccountDialog::createAccount() {
    QString username = ui->usernameInput->text();
    QString password = ui->passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić wszystkie pola.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO user (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec()) {
        QMessageBox::information(this, "Sukces", "Konto zostało utworzone.");
        accept();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się utworzyć konta.");
    }
}

CreateAccountDialog::~CreateAccountDialog() {
    delete ui;
}

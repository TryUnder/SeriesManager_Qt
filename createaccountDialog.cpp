#include "createaccountDialog.h"
#include "ui_createaccount.h"
#include "accountmanager.h"
#include <QSqlQuery>
#include <QMessageBox>

CreateAccountDialog::CreateAccountDialog(DatabaseManager* dbManager, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CreateAccountDialog),
    m_dbManager(dbManager)
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

    accountmanager accountManager(m_dbManager);
    if (accountManager.createAccount(username, password)) {
        QMessageBox::information(this, "Sukces", "Konto zostało utworzone");
        accept();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się utworzyć konta.");
    }
}

CreateAccountDialog::~CreateAccountDialog() {
    delete ui;
}

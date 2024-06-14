#include "deleteaccount.h"
#include "ui_deleteaccount.h"

DeleteAccount::DeleteAccount(DatabaseManager* dbManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteAccount),
    m_dbManager(dbManager)
{
    ui->setupUi(this);

    connect(ui->deleteAccount, &QPushButton::clicked, this, &DeleteAccount::deleteAccount);
    connect(ui->deleteAccountCancel, &QPushButton::clicked, this, &DeleteAccount::reject);
}

bool DeleteAccount::deleteAccount() {
    QString username = ui->usernameInput->text();
    QString password = ui->passwordOutput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę uzupełnić wszystkie pola.");
        return false;
    }

    accountmanager accountManager(m_dbManager);
    if (accountManager.deleteAccount(username, password)) {
        QMessageBox::information(this, "Sukces", "Pomyślnie usunięto konto.");
        accept();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć konta.");
        return false;
    }

    return false;
}

DeleteAccount::~DeleteAccount()
{
    delete ui;
}

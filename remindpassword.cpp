#include "remindpassword.h"
#include "ui_remindpassword.h"

RemindPassword::RemindPassword(DatabaseManager* dbManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemindPassword),
    m_dbManager(dbManager)
{
    ui->setupUi(this);
    ui->passwordOutput->setDisabled(true);

    connect(ui->remindButton, &QPushButton::clicked, this, &RemindPassword::remindPassword);
    connect(ui->remindButtonCancel, &QPushButton::clicked, this, &QDialog::reject);
}

void RemindPassword::remindPassword() {
    QString username = ui->usernameInput->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę podać login.");
        return;
    }

    accountmanager accountManager(m_dbManager);
    QPair<bool, QString> remindedPassword = accountManager.remindPassword(username);
    if (remindedPassword.first == true) {
        QMessageBox::information(this, "Sukces", "Udało się przypomnieć hasło.");
        ui->passwordOutput->setText(remindedPassword.second);
    } else {
        QMessageBox::warning(this, "Błąd", "Błędny login");
    }
}

RemindPassword::~RemindPassword()
{
    delete ui;
}

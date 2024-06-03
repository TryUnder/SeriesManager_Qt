#include "LoginAccountDialog.h"
#include "ui_loginAccount.h"

LoginAccountDialog::LoginAccountDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoginAccountDialog)
{
    ui->setupUi(this);
}

LoginAccountDialog::~LoginAccountDialog() {
    delete ui;
}

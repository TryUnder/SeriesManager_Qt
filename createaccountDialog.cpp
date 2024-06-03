#include "createaccountDialog.h"
#include "ui_createaccount.h"

CreateAccountDialog::CreateAccountDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CreateAccountDialog)
{
    ui->setupUi(this);
}

CreateAccountDialog::~CreateAccountDialog() {
    delete ui;
}

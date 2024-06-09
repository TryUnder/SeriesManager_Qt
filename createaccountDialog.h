#ifndef CREATEACCOUNTDIALOG_H
#define CREATEACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateAccountDialog;
}

class CreateAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAccountDialog(QWidget *parent = nullptr);
    ~CreateAccountDialog();

    void createAccount();

public:
    Ui::CreateAccountDialog *ui;
};

#endif // CREATEACCOUNTDIALOG_H

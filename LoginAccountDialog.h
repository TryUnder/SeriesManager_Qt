#ifndef LOGINACCOUNTDIALOG_H
#define LOGINACCOUNTDIALOG_H


#include <QDialog>

namespace Ui {
    class LoginAccountDialog;
}

class LoginAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginAccountDialog(QWidget *parent = nullptr);
    ~LoginAccountDialog();

    void loginAccount();

public:
    Ui::LoginAccountDialog *ui;
};

#endif // LOGINACCOUNTDIALOG_H

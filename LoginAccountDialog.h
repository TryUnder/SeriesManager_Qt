#ifndef LOGINACCOUNTDIALOG_H
#define LOGINACCOUNTDIALOG_H


#include <QDialog>
#include "databasemanager.h"

namespace Ui {
    class LoginAccountDialog;
}

class LoginAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginAccountDialog(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~LoginAccountDialog();

    void loginAccount();

public:
    Ui::LoginAccountDialog *ui;
    DatabaseManager* m_dbManager;
};

#endif // LOGINACCOUNTDIALOG_H

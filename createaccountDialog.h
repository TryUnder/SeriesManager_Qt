#ifndef CREATEACCOUNTDIALOG_H
#define CREATEACCOUNTDIALOG_H

#include <QDialog>
#include "databasemanager.h"

namespace Ui {
    class CreateAccountDialog;
}

class CreateAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAccountDialog(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~CreateAccountDialog();

    void createAccount();

public:
    Ui::CreateAccountDialog *ui;
    DatabaseManager* m_dbManager;
};

#endif // CREATEACCOUNTDIALOG_H

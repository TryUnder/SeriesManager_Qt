#ifndef DELETEACCOUNT_H
#define DELETEACCOUNT_H

#include <QDialog>
#include "databasemanager.h"
#include <QMessageBox>
#include "accountmanager.h"

namespace Ui {
class DeleteAccount;
}

class DeleteAccount : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAccount(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~DeleteAccount();

    bool deleteAccount();

private:
    Ui::DeleteAccount *ui;
    DatabaseManager* m_dbManager;
};

#endif // DELETEACCOUNT_H

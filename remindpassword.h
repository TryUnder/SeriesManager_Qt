#ifndef REMINDPASSWORD_H
#define REMINDPASSWORD_H

#include <QDialog>
#include "databasemanager.h"
#include <QMessageBox>
#include "accountmanager.h"
#include <QPair>

namespace Ui {
class RemindPassword;
}

class RemindPassword : public QDialog
{
    Q_OBJECT

public:
    explicit RemindPassword(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~RemindPassword();

    void remindPassword();

private:
    Ui::RemindPassword *ui;
    DatabaseManager* m_dbManager;
};

#endif // REMINDPASSWORD_H

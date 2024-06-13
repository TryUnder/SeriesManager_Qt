#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include "databasemanager.h"

class accountmanager : public QObject
{
    Q_OBJECT

public:
    explicit accountmanager(DatabaseManager* dbManager, QObject* parent = nullptr);
    bool createAccount(const QString& username, const QString& password);
    bool loginAccount(const QString& username, const QString& password);
    QPair<bool, QString> remindPassword(const QString& username);

private:
    DatabaseManager* m_dbManager;
};

#endif // ACCOUNTMANAGER_H

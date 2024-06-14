#include "accountmanager.h"
#include <QMessageBox>


accountmanager::accountmanager(DatabaseManager* dbManager, QObject* parent)
    : QObject(parent), m_dbManager(dbManager) { }

bool accountmanager::createAccount(const QString& username, const QString& password) {
    return m_dbManager->createAccount(username, password);
}

bool accountmanager::loginAccount(const QString &username, const QString &password) {
    return m_dbManager->loginAccount(username, password);
}

QPair<bool, QString> accountmanager::remindPassword(const QString& username) {
    return m_dbManager->remindPassword(username);
}

bool accountmanager::deleteAccount(const QString &username, const QString &password) {
    return m_dbManager->deleteAccount(username, password);
}

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QVector>
#include "series.h"

class DatabaseManager
{
public:
    DatabaseManager(const QString& pathToDb);
    ~DatabaseManager();

    bool addSeries(const Series& series);
    QVector<Series> getSeries() const;
    Series getSeriesById(int id) const;
    bool updateSeries(const Series& series);
    bool deleteSeries(int id);

    bool doesUserExist();
    bool createAccount(const QString& username, const QString& password);
    bool loginAccount(const QString& username, const QString& password);
    QPair<bool, QString> remindPassword(const QString& username);
private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H

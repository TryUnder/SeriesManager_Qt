#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QVector>
#include "series.h"
#include <QDebug>
#include <QSqlError>
#include <QPair>
#include <QStandardPaths>
#include <QDir>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool checkTables();
    void createTables();

    bool addSeries(const Series& series);
    QVector<Series> getSeries() const;
    Series getSeriesById(int id) const;
    bool updateSeries(const Series& series);
    bool deleteSeries(int id);

    QPair<bool, QVector<Series>> getSeriesByTitle(const QString& title);
    QPair<bool, QVector<Series>> getSeriesByGenre(const QString& genre);
    QPair<bool, QVector<Series>> getSeriesByCategory(const QString& category);
    QPair<bool, QVector<Series>> getSeriesByGrade(const int& grade);

    bool doesUserExist();
    bool createAccount(const QString& username, const QString& password);
    bool loginAccount(const QString& username, const QString& password);
    QPair<bool, QString> remindPassword(const QString& username);
    bool deleteAccount(const QString& username, const QString& password);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H

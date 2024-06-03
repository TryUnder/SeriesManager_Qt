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
    bool updateSeries(const Series& series);
    bool deleteSeries(int id);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H

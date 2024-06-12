#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager(const QString& pathToDb)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(pathToDb);

    if (!m_db.open()) {
        qDebug() << "Database connection error: " << m_db.lastError().text();
    }    
}

bool DatabaseManager::doesUserExist() {
    QSqlQuery query("SELECT COUNT(*) FROM user");
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt() > 0;
        }
    }

    return false;
}

bool DatabaseManager::addSeries(const Series& series) {
    QSqlQuery query;
    query.prepare("INSERT INTO Series (title, genre, starting_date, ending_date, episodes_watched, url, category, grade)"
                  "VALUES (:title, :genre, :starting_date, :ending_date, :episodes_watched, :url, :category, :grade)");
    query.bindValue(":title", series.getTitle());
    query.bindValue(":genre", series.getGenre());
    query.bindValue(":starting_date", series.getStartingDate().toString("yyyy-MM-dd"));
    query.bindValue(":ending_date", series.getEndingDate().toString("yyyy-MM-dd"));
    query.bindValue(":episodes_watched", series.getEpisodesWatched());
    query.bindValue(":url", series.getUrl());
    query.bindValue(":category", series.getCategory());
    query.bindValue(":grade", series.getGrade());

    return query.exec();
}

QVector<Series> DatabaseManager::getSeries() const {
    QVector<Series> seriesList;
    QSqlQuery query("SELECT id, title, genre, starting_date, ending_date, episodes_watched, url, category, grade FROM Series");

    while(query.next()) {
        Series series(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("genre").toString(),
            query.value("starting_date").toDate(),
            query.value("ending_date").toDate(),
            query.value("episodes_watched").toInt(),
            query.value("url").toString(),
            query.value("category").toString(),
            query.value("grade").toInt()
        );
        seriesList.append(series);
    }

    return seriesList;
}

bool DatabaseManager::deleteSeries(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Series WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

DatabaseManager::~DatabaseManager() {
    m_db.close();
}

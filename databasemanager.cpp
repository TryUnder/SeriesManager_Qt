#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager(const QString& pathToDb)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(pathToDb);

    if (!m_db.open()) {
        qDebug() << "Database connection error: " << m_db.lastError().text();
    } else {
        qDebug() << "Database connection succesfull.";
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
    query.bindValue(":starting_date", series.getStartingDate());
    query.bindValue(":ending_date", series.getEndingDate());
    query.bindValue(":episodes_watched", series.getEpisodesWatched());
    query.bindValue(":url", series.getUrl());
    query.bindValue(":category", series.getCategory());
    query.bindValue(":grade", series.getGrade());

    return query.exec();
}

QVector<Series> DatabaseManager::getSeries() const {
    qDebug() << "JEST";
    QVector<Series> seriesList;
    QSqlQuery query("SELECT id, title, genre, starting_date, ending_date, episodes_watched, url, category, grade FROM Series");

    while(query.next()) {
        Series series(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("genre").toString(),
            query.value("starting_date").toString(),
            query.value("ending_date").toString(),
            query.value("episodes_watched").toInt(),
            query.value("url").toString(),
            query.value("category").toString(),
            query.value("grade").toInt()
        );
        seriesList.append(series);
    }
    qDebug() << "JEST_2";
    return seriesList;
}

Series DatabaseManager::getSeriesById(int id) const {
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, url, category, grade FROM Series WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Błąd wykonania zapytania: " << query.lastError().text();
        throw std::runtime_error("Błąd przy pobieraniu danych serialu: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        int id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString genre = query.value("genre").toString();
        QString starting_date = query.value("starting_date").toString();
        QString ending_date = query.value("ending_date").toString();
        int episodesWatched = query.value("episodes_watched").toInt();
        QString url = query.value("url").toString();
        QString status = query.value("category").toString();
        int grade = query.value("grade").toInt();

        Series series(id, title, genre, starting_date, ending_date, episodesWatched, url, status, grade);
        return series;
    } else {
        throw std::runtime_error("Nie znaleziono serialu o podanym id");
    }
}

bool DatabaseManager::deleteSeries(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM Series WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

bool DatabaseManager::updateSeries(const Series& series) {
    QSqlQuery query;
    query.prepare("UPDATE Series SET title = :title, genre = :genre, starting_date = :starting_date, ending_date = :ending_date, "
                  "episodes_watched = :episodes_watched, url = :url, category = :category, grade = :grade WHERE id = :id");
    query.bindValue(":title", series.getTitle());
    query.bindValue(":genre", series.getGenre());
    query.bindValue(":starting_date", series.getStartingDate());
    query.bindValue(":ending_date", series.getEndingDate());
    query.bindValue(":episodes_watched", series.getEpisodesWatched());
    query.bindValue(":url", series.getUrl());
    query.bindValue(":category", series.getCategory());
    query.bindValue(":grade", series.getGrade());
    query.bindValue(":id", series.getId());

    return query.exec();
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

#include "databasemanager.h"


DatabaseManager::DatabaseManager()
{
    QString dbPath;

    if (QSysInfo::productType() == "windows") {
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir dir(appDataPath);

        if (!dir.exists()) {
            dir.mkpath(appDataPath);
        }

        dbPath = appDataPath + QDir::separator() + "Serialnik.db";
    } else {
        dbPath = QDir::tempPath() + QDir::separator() + "Serialnik.db";
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "Database connection error: " << m_db.lastError().text();
    } else {
        qDebug() << "Database connection succesfull.";
    }

    if (!checkTables()) {
        createTables();
    }
}

bool DatabaseManager::checkTables() {
    QStringList requiredTables = {"user", "Series"};
    QSqlQuery query;

    foreach (const QString& tableName, requiredTables) {
        query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name = :tableName");
        query.bindValue(":tableName", tableName);

        if (!query.exec() || !query.next()) {
            qDebug() << "Tabela: " << tableName << " nie istnieje.";
            return false;
        }
    }

    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery query;

    QString createTableSeries = "CREATE TABLE IF NOT EXISTS Series ("
                                "id	INTEGER NOT NULL,"
                                "title	TEXT UNIQUE NOT NULL,"
                                "genre	TEXT,"
                                "starting_date TEXT,"
                                "ending_date TEXT,"
                                "episodes_watched INTEGER,"
                                "all_episodes INTEGER,"
                                "url TEXT,"
                                "category TEXT,"
                                "grade INTEGER,"
                                "PRIMARY KEY(id AUTOINCREMENT)"
                            ")";

    QString createTableUser = "CREATE TABLE IF NOT EXISTS user ("
                "username TEXT NOT NULL,"
                "password TEXT NOT NULL"
            ")";

    if (!query.exec(createTableSeries)) {
        qDebug() << "Nie udało się utworzyć tabeli z serialami." << query.lastError().text();
    } else {
        qDebug() << "Pomyślnie utworzono tabelę z serialami.";
    }

    if (!query.exec(createTableUser)) {
        qDebug() << "Nie udało się utworzyć tabeli użytkownika." << query.lastError().text();
    } else {
        qDebug() << "Pomyślnie utworzono tabelę użytkownika.";
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

bool DatabaseManager::createAccount(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO user (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    return query.exec();
}

bool DatabaseManager::loginAccount(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM user WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

QPair<bool, QString> DatabaseManager::remindPassword(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT password FROM user WHERE username = :username");
    query.bindValue(":username", username);
    QPair<bool, QString> remindedPasswordPair;

    if (query.exec() && query.next()) {
        remindedPasswordPair.first = true;
        remindedPasswordPair.second = query.value("password").toString();
    } else {
        remindedPasswordPair.first = false;
        remindedPasswordPair.second = "";
    }
    return remindedPasswordPair;
}

bool DatabaseManager::deleteAccount(const QString &username, const QString &password) {
    if (!m_db.transaction()) {
        qDebug() << "Nie udało się rozpocząć transakcji.";
        return false;
    }

    QSqlQuery deleteAccount, deleteSeries;

    deleteAccount.prepare("DELETE FROM user WHERE username LIKE :username AND password LIKE :password");
    deleteAccount.bindValue(":username", username);
    deleteAccount.bindValue(":password", password);

    if (!deleteAccount.exec()) {
        qDebug() << "Nie udało się usunąć konta użytkownika." << deleteAccount.lastError().text();
        m_db.rollback();
        return false;
    }

    if (deleteAccount.numRowsAffected() == 0) {
        qDebug() << "Nie znaleziono konta użytkownika do usunięcia.";
        m_db.rollback();
        return false;
    }

    deleteSeries.prepare("DELETE FROM Series");

    if (!deleteSeries.exec()) {
        qDebug() << "Nie udało się usunąć seriali." << deleteSeries.lastError().text();
        m_db.rollback();
        return false;
    }

    if (!m_db.commit()) {
        qDebug() << "Nie udało się zatwierdzić transakcji.";
        m_db.rollback();
        return false;
    }

    return true;
}

bool DatabaseManager::addSeries(const Series& series) {
    QSqlQuery query;
    query.prepare("INSERT INTO Series (title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade)"
                  "VALUES (:title, :genre, :starting_date, :ending_date, :episodes_watched, :all_episodes, :url, :category, :grade)");
    query.bindValue(":title", series.getTitle());
    query.bindValue(":genre", series.getGenre());
    query.bindValue(":starting_date", series.getStartingDate());
    query.bindValue(":ending_date", series.getEndingDate());
    query.bindValue(":episodes_watched", series.getEpisodesWatched());
    query.bindValue(":all_episodes", series.getAllEpisodes());
    query.bindValue(":url", series.getUrl());
    query.bindValue(":category", series.getCategory());
    query.bindValue(":grade", series.getGrade());

    return query.exec();
}

QVector<Series> DatabaseManager::getSeries() const {
    QVector<Series> seriesList;
    QSqlQuery query("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade FROM Series");

    while(query.next()) {
        Series series(
            query.value("id").toInt(),
            query.value("title").toString(),
            query.value("genre").toString(),
            query.value("starting_date").toString(),
            query.value("ending_date").toString(),
            query.value("episodes_watched").toInt(),
            query.value("all_episodes").toInt(),
            query.value("url").toString(),
            query.value("category").toString(),
            query.value("grade").toInt()
        );
        seriesList.append(series);
    }

    return seriesList;
}

Series DatabaseManager::getSeriesById(int id) const {
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade FROM Series WHERE id = :id");
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
        int allEpisodes = query.value("all_episodes").toInt();
        QString url = query.value("url").toString();
        QString status = query.value("category").toString();
        int grade = query.value("grade").toInt();

        Series series(id, title, genre, starting_date, ending_date, episodesWatched, allEpisodes, url, status, grade);
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
                  "episodes_watched = :episodes_watched, all_episodes = :all_episodes, url = :url, category = :category, grade = :grade WHERE id = :id");
    query.bindValue(":title", series.getTitle());
    query.bindValue(":genre", series.getGenre());
    query.bindValue(":starting_date", series.getStartingDate());
    query.bindValue(":ending_date", series.getEndingDate());
    query.bindValue(":episodes_watched", series.getEpisodesWatched());
    query.bindValue(":all_episodes", series.getAllEpisodes());
    query.bindValue(":url", series.getUrl());
    query.bindValue(":category", series.getCategory());
    query.bindValue(":grade", series.getGrade());
    query.bindValue(":id", series.getId());

    return query.exec();
}

QPair<bool, QVector<Series>> DatabaseManager::getSeriesByTitle(const QString& title) {
    QVector<Series> seriesList;
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade "
                  "FROM Series WHERE title LIKE :title");
    query.bindValue(":title", "%" + title + "%");

    if (!query.exec()) {
        qDebug() << "Błąd wykonywania zapytania: " << query.lastError().text();
        return qMakePair(false, seriesList);
    }

    while(query.next()) {
            Series series(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("genre").toString(),
                query.value("starting_date").toString(),
                query.value("ending_date").toString(),
                query.value("episodes_watched").toInt(),
                query.value("all_episodes").toInt(),
                query.value("url").toString(),
                query.value("category").toString(),
                query.value("grade").toInt()
            );
            seriesList.append(series);
        }

    return qMakePair(true, seriesList);
}

QPair<bool, QVector<Series>> DatabaseManager::getSeriesByGenre(const QString &genre) {
    QVector<Series> seriesList;
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade "
                  "FROM Series WHERE genre LIKE :genre");
    query.bindValue(":genre", "%" + genre + "%");

    if (!query.exec()) {
        qDebug() << "Błąd wykonywania zapytania: " << query.lastError().text();
        return qMakePair(false, seriesList);
    }

    while(query.next()) {
            Series series(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("genre").toString(),
                query.value("starting_date").toString(),
                query.value("ending_date").toString(),
                query.value("episodes_watched").toInt(),
                query.value("all_episodes").toInt(),
                query.value("url").toString(),
                query.value("category").toString(),
                query.value("grade").toInt()
            );
            seriesList.append(series);
        }

    return qMakePair(true, seriesList);
}

QPair<bool, QVector<Series>> DatabaseManager::getSeriesByCategory(const QString& category) {
    QVector<Series> seriesList;
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade "
                  "FROM Series WHERE category LIKE :category");
    query.bindValue(":category", category);

    if (!query.exec()) {
        qDebug() << "Błąd wykonywania zapytania: " << query.lastError().text();
        return qMakePair(false, seriesList);
    }

    while(query.next()) {
            Series series(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("genre").toString(),
                query.value("starting_date").toString(),
                query.value("ending_date").toString(),
                query.value("episodes_watched").toInt(),
                query.value("all_episodes").toInt(),
                query.value("url").toString(),
                query.value("category").toString(),
                query.value("grade").toInt()
            );
            seriesList.append(series);
        }

    return qMakePair(true, seriesList);
}

QPair<bool, QVector<Series>> DatabaseManager::getSeriesByGrade(const int& grade) {
    QVector<Series> seriesList;
    QSqlQuery query;
    query.prepare("SELECT id, title, genre, starting_date, ending_date, episodes_watched, all_episodes, url, category, grade "
                  "FROM Series WHERE grade LIKE :grade");
    query.bindValue(":grade", grade);

    if (!query.exec()) {
        qDebug() << "Błąd wykonywania zapytania: " << query.lastError().text();
        return qMakePair(false, seriesList);
    }

    while(query.next()) {
            Series series(
                query.value("id").toInt(),
                query.value("title").toString(),
                query.value("genre").toString(),
                query.value("starting_date").toString(),
                query.value("ending_date").toString(),
                query.value("episodes_watched").toInt(),
                query.value("all_episodes").toInt(),
                query.value("url").toString(),
                query.value("category").toString(),
                query.value("grade").toInt()
            );
            seriesList.append(series);
        }

    return qMakePair(true, seriesList);
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

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

    QSqlQuery query("SELECT * FROM Series");
    query.exec();

        while (query.next()) {
            QString id = query.value("id").toString();
            QString title = query.value("title").toString();
            QString genre = query.value("genre").toString();
            QString starting_date = query.value("starting_date").toString();
            QString ending_date = query.value("ending_date").toString();
            QString episodes_watched = query.value("episodes_watched").toString();
            QString url = query.value("url").toString();
            QString category = query.value("category").toString();
            QString grade = query.value("grade").toString();

            qDebug() << "ID:" << id;
            qDebug() << "Tytuł:" << title;
            qDebug() << "Gatunek:" << genre;
            qDebug() << "Data rozpoczęcia:" << starting_date;
            qDebug() << "Data zakończenia:" << ending_date;
            qDebug() << "Obejrzane odcinki:" << episodes_watched;
            qDebug() << "URL:" << url;
            qDebug() << "Kategoria:" << category;
            qDebug() << "Ocena:" << grade;
            qDebug() << "---------------------------";
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

DatabaseManager::~DatabaseManager() {
    m_db.close();
}

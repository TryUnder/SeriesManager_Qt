#include "databasemanager.h"
#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager(const QString& host, const QString& user, const QString& password, const QString& dbName)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName(host);
    m_db.setUserName(user);
    m_db.setPassword(password);
    m_db.setDatabaseName(dbName);

    if (!m_db.open()) {
        qDebug() << "Database connection error: " << m_db.lastError().text();
    }

    QSqlQuery query("SELECT * FROM seriale");

        while (query.next()) {
            QString id = query.value("id").toString();
            QString tytul = query.value("tytul").toString();
            QString gatunek = query.value("gatunek").toString();
            QString data_rozpoczecia = query.value("data_rozpoczecia").toString();
            QString data_zakonczenia = query.value("data_zakonczenia").toString();
            QString obejrzane_odcinki = query.value("obejrzane_odcinki").toString();
            QString url = query.value("url").toString();
            QString kategoria = query.value("kategoria").toString();
            QString ocena = query.value("ocena").toString();

            qDebug() << "ID:" << id;
            qDebug() << "Tytuł:" << tytul;
            qDebug() << "Gatunek:" << gatunek;
            qDebug() << "Data rozpoczęcia:" << data_rozpoczecia;
            qDebug() << "Data zakończenia:" << data_zakonczenia;
            qDebug() << "Obejrzane odcinki:" << obejrzane_odcinki;
            qDebug() << "URL:" << url;
            qDebug() << "Kategoria:" << kategoria;
            qDebug() << "Ocena:" << ocena;
            qDebug() << "---------------------------";
        }
}

DatabaseManager::~DatabaseManager() {
    m_db.close();
}

#ifndef SERIES_H
#define SERIES_H

#include <QString>
#include <QDate>

class Series
{
public:
    int m_id;
    QString m_title;
    QString m_genre;
    QDate m_startingDate;
    QDate m_endingDate;
    int m_episodesWatched;
    QString m_url;
    QString m_category;
    int m_grade;

    Series(int id, QString title, QString genre, QDate startingDate, QDate endingDate,
           int episodesWatched, QString url, QString category, int grade);
};

#endif // SERIES_H

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
           int episodesWatched, QString url, QString category, int grade) :
        m_id {id}, m_title {title}, m_genre {genre}, m_startingDate {startingDate}, m_endingDate {endingDate},
        m_episodesWatched {episodesWatched}, m_url {url}, m_category {category}, m_grade {grade} {}
};

#endif // SERIES_H

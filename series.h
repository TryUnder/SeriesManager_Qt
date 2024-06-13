#ifndef SERIES_H
#define SERIES_H

#include <QString>
#include <QDate>

class Series
{
private:
    int m_id;
    QString m_title;
    QString m_genre;
    QString m_startingDate;
    QString m_endingDate;
    int m_episodesWatched;
    QString m_url;
    QString m_category;
    int m_grade;

public:
    Series(int id, QString title, QString genre, QString startingDate, QString endingDate,
           int episodesWatched, QString url, QString category, int grade);
    Series();

    int getId() const;
    QString getTitle() const;
    QString getGenre() const;
    QString getStartingDate() const;
    QString getEndingDate() const;
    QString getEpisodesWatched() const;
    QString getUrl() const;
    QString getCategory() const;
    QString getGrade() const;
};

#endif // SERIES_H

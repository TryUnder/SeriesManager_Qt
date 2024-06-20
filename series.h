#ifndef SERIES_H
#define SERIES_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QDebug>

class Series
{
private:
    int m_id;
    QString m_title;
    QString m_genre;
    QString m_startingDate;
    QString m_endingDate;
    int m_episodesWatched;
    int m_allEpisodes;
    QString m_discLocation;
    QString m_category;
    int m_grade;

public:
    Series(int id, QString title, QString genre, QString startingDate, QString endingDate,
           int episodesWatched, int allEpisodes, QString discLocation, QString category, int grade);
    Series();

    enum class Category {
        Watching,
        Completed,
        Dropped,
        OnHold,
        PlanToWatch
    };

    static QString categoryToPolish(Category category);
    static Category categoryFromEnglish(const QString& category);

    int getId() const;
    QString getTitle() const;
    QString getGenre() const;
    QString getStartingDate() const;
    QString getEndingDate() const;
    QString getEpisodesWatched() const;
    QString getAllEpisodes() const;
    QString getDiscLocation() const;
    QString getCategory() const;
    QString getGrade() const;

    void setTitle(QString title);
    void setStartingDate(QString startingDate);
    void setEndingDate(QString endingDate);
    void setGrade(int grade);
    void setCategory(QString category);
    void setEpisodesWatched(int episodesWatched);
    void setAllEpisodes(int allEpisodes);

    static bool isDefaultDate(const QString& date);
};

#endif // SERIES_H

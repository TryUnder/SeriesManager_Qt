#include "series.h"

Series::Series(int id, QString title, QString genre, QString startingDate, QString endingDate,
                      int episodesWatched, QString url, QString category, int grade) :
                   m_id {id}, m_title {title}, m_genre {genre}, m_startingDate {startingDate}, m_endingDate {endingDate},
                   m_episodesWatched {episodesWatched}, m_url {url}, m_category {category}, m_grade {grade} {}
Series::Series() : m_id(-1), m_episodesWatched(0), m_grade(0) {}

int Series::getId() const { return m_id; }
QString Series::getTitle() const { return m_title; }
QString Series::getGenre() const { return m_genre; }
QString Series::getStartingDate() const { return m_startingDate; }
QString Series::getEndingDate() const { return m_endingDate; }
QString Series::getEpisodesWatched() const { return QString::number(m_episodesWatched); }
QString Series::getUrl() const { return m_url; }
QString Series::getCategory() const { return m_category; }
QString Series::getGrade() const { return QString::number(m_grade); }

void Series::setTitle(QString title) { m_title = title; }
void Series::setEpisodesWatched(int episodesWatched) { m_episodesWatched = episodesWatched; }
void Series::setStartingDate(QString startingDate) { m_startingDate = startingDate; }
void Series::setEndingDate(QString endingDate) { m_endingDate = endingDate; }
void Series::setGrade(int grade) { m_grade = grade; }
void Series::setCategory(QString category) { m_category = category; }

QString Series::categoryToPolish(Series::Category category) {
    switch (category) {
        case Category::Watching: return "Oglądane";
        case Category::Completed: return "Zakończone";
        case Category::Dropped: return "Porzucone";
        case Category::OnHold: return "Wstrzymane";
        case Category::PlanToWatch: return "Planowane";
        default: return "Nieznana kategoria";
    };
}

Series::Category Series::categoryFromEnglish(const QString &category) {
    static QMap<QString, Category> categoryMap = {
        { "Watching", Category::Watching },
        { "Completed", Category::Completed },
        { "Dropped", Category::Dropped },
        { "On-Hold", Category::OnHold },
        { "Plan to Watch", Category::PlanToWatch }
    };

    return categoryMap.value(category, Category::Watching);
}

bool Series::isDefaultDate(const QString &date) {
    return date == "0000-00-00";
}

#include "series.h"

Series::Series(int id, QString title, QString genre, QString startingDate, QString endingDate,
                      int episodesWatched, int allEpisodes, QString discLocation, QString category, int grade) :
                   m_id {id}, m_title {title}, m_genre {genre}, m_startingDate {startingDate}, m_endingDate {endingDate},
                   m_episodesWatched {episodesWatched}, m_allEpisodes {allEpisodes}, m_discLocation {discLocation}, m_category {category}, m_grade {grade} {}
Series::Series() : m_id(-1), m_episodesWatched(0), m_grade(0) {}

int Series::getId() const { return m_id; }
QString Series::getTitle() const { return m_title; }
QString Series::getGenre() const { return m_genre; }
QString Series::getStartingDate() const { return m_startingDate; }
QString Series::getEndingDate() const { return m_endingDate; }
QString Series::getEpisodesWatched() const { return QString::number(m_episodesWatched); }
QString Series::getAllEpisodes() const { return QString::number(m_allEpisodes); }
QString Series::getDiscLocation() const { return m_discLocation; }
QString Series::getCategory() const { return m_category; }
QString Series::getGrade() const { return QString::number(m_grade); }

void Series::setTitle(QString title) { m_title = title; }
void Series::setEpisodesWatched(int episodesWatched) { m_episodesWatched = episodesWatched; }
void Series::setAllEpisodes(int allEpisodes) { m_allEpisodes = allEpisodes; }
void Series::setStartingDate(QString startingDate) {
    if (startingDate != "0000-00-00") {
        QDate date = QDate::fromString(startingDate, "yyyy-MM-dd");
        QString formattedStartingDate = date.toString("dd-MM-yyyy");
        m_startingDate = formattedStartingDate;
    } else {
        m_startingDate = startingDate;
    }
}
void Series::setEndingDate(QString endingDate) {
    if (endingDate != "0000-00-00") {
        QDate date = QDate::fromString(endingDate, "yyyy-MM-dd");
        QString formattedEndingDate = date.toString("dd-MM-yyyy");
        m_endingDate = formattedEndingDate;
    } else {
        m_startingDate = endingDate;
    }
}
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

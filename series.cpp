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

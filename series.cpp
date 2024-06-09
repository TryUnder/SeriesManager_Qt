#include "series.h"

Series::Series(int id, QString title, QString genre, QDate startingDate, QDate endingDate,
                      int episodesWatched, QString url, QString category, int grade) :
                   m_id {id}, m_title {title}, m_genre {genre}, m_startingDate {startingDate}, m_endingDate {endingDate},
                   m_episodesWatched {episodesWatched}, m_url {url}, m_category {category}, m_grade {grade} {}

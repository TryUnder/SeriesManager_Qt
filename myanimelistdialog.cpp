#include "myanimelistdialog.h"
#include "ui_myanimelistdialog.h"

MyAnimeListDialog::MyAnimeListDialog(DatabaseManager* dbManager, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::MyAnimeListDialog),
    m_dbManager(dbManager)
{
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &MyAnimeListDialog::onBrowseButtonClicked);
    connect(ui->importButton, &QPushButton::clicked, this, &MyAnimeListDialog::onImportButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &MyAnimeListDialog::reject);
}

void MyAnimeListDialog::setMyAnimeListXmlPath(QString myAnimeListXmlPath) {
    m_myAnimeListXmlPath = myAnimeListXmlPath;
}

QString MyAnimeListDialog::getMyAnimeListXmlPath() {
    return m_myAnimeListXmlPath;
}

void MyAnimeListDialog::onBrowseButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik XML do importu", "", "XML Files (*.xml)");
    if (!filePath.isEmpty()) {
        ui->filePathInput->setText(filePath);
        setMyAnimeListXmlPath(filePath);
    }
}

void MyAnimeListDialog::onImportButtonClicked() {
    if (m_myAnimeListXmlPath.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wybrać plik XML.");
        return;
    }

    if (importFromXML()) {
        QMessageBox::information(this, "Sukces", "Import zakończony sukcesem");
        emit importSuccesfull();
    } else {
        QMessageBox::critical(this, "Błąd", "Wystąpił błąd podczas importu pliku XML.");
    }
}

bool MyAnimeListDialog::importFromXML() {
    QFile file(m_myAnimeListXmlPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie udało się otworzyć pliku";
        return false;
    }

    QXmlStreamReader xml(&file);
    QVector<Series> seriesList;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "anime") {
                Series series;
                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "anime")) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement) {
                        if (xml.name() == "series_title") {
                            series.setTitle(xml.readElementText());
                        }

                        if (xml.name() == "my_watched_episodes") {
                            series.setEpisodesWatched(xml.readElementText().toInt());
                        }

                        if (xml.name() == "my_start_date") {
                            series.setStartingDate(xml.readElementText());
                        }

                        if (xml.name() == "my_finish_date") {
                            series.setEndingDate(xml.readElementText());
                        }

                        if (xml.name() == "my_score") {
                            series.setGrade(xml.readElementText().toInt());
                        }

                        if (xml.name() == "my_status") {
                            QString category = Series::categoryToPolish(Series::categoryFromEnglish(xml.readElementText()));
                            series.setCategory(category);
                        }
                    }
                    xml.readNext();
                }
                seriesList.append(series);
            }
        }
    }

    if (seriesList.size() > 0) {
        for (const Series& series : seriesList) {
            m_dbManager->addSeries(series);
        }
    }

    if (xml.hasError()) {
        qDebug() << "Błąd przy parsowaniu XML: " << xml.errorString();
        return false;
    }

    close();
    return true;
}

MyAnimeListDialog::~MyAnimeListDialog()
{
    delete ui;
}

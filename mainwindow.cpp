#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountDialog.h"
#include "LoginAccountDialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbManager()
    , isLoggedIn(false)
    , accountManager(&dbManager)
{
    ui->setupUi(this);
    ui->genreInput->setMaximumWidth(75);
    ui->genreInput->setMaximumHeight(50);
    connect(ui->createAccountAction, &QAction::triggered, this, &MainWindow::createAccount);
    connect(ui->loginAccountAction, &QAction::triggered, this, &MainWindow::loginAccount);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addSeries);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::removeSeries);
    connect(ui->actionPrzypomnijHaslo, &QAction::triggered, this, &MainWindow::remindPassword);
    connect(ui->actionWyloguj, &QAction::triggered, this, &MainWindow::logoutUser);
    connect(ui->actiondeleteAccount, &QAction::triggered, this, &MainWindow::deleteAccount);
    connect(ui->actionMyAnimeListOpenDialog, &QAction::triggered, this, &MainWindow::openMyAnimeListImportDialog);

    connect(ui->actionPo_tytule, &QAction::triggered, this, &MainWindow::searchSeriesByTitle);
    connect(ui->actionPo_gatunku, &QAction::triggered, this, &MainWindow::searchSeriesByGenre);
    connect(ui->actionPo_kategorii, &QAction::triggered, this, &MainWindow::searchSeriesByCategory);
    connect(ui->actionPo_ocenie, &QAction::triggered, this, &MainWindow::searchSeriesByGrade);
    connect(ui->actionShowAll, &QAction::triggered, this, &MainWindow::loadAllSeries);

    ui->seriesTable->setSortingEnabled(true);

    updateUIBasedOnLoginStatus();
}

void MainWindow::createAccount() {
    if (!dbManager.doesUserExist()) {
        CreateAccountDialog createAccountDialog(&dbManager, this);
        createAccountDialog.exec();
    } else {
        QMessageBox::information(this, "Informacja", "Konto już istnieje. Zaloguj się.");
    }
}

void MainWindow::loginAccount() {
    LoginAccountDialog loginAccountDialog(&dbManager, this);

    if (isLoggedIn || !dbManager.doesUserExist()) {
        return;
    }

    if (loginAccountDialog.exec() == QDialog::Accepted) {
        isLoggedIn = true;
        QMessageBox::information(this, "Sukces", "Zalogowano pomyślnie");
        updateUIBasedOnLoginStatus();
    }
}

void MainWindow::remindPassword() {
    RemindPassword remindPassword(&dbManager, this);

    if (isLoggedIn) {
        return;
    }

    remindPassword.exec();
}

void MainWindow::logoutUser() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Nie jesteś zalogowany");
        return;
    }

    QMessageBox::information(this, "Sukces", "Wylogowano pomyślnie.");

    isLoggedIn = false;
    updateUIBasedOnLoginStatus();
}

void MainWindow::deleteAccount() {
    DeleteAccount deleteAccount(&dbManager);

    if (deleteAccount.exec() == QDialog::Accepted) {
        isLoggedIn = false;
        updateUIBasedOnLoginStatus();
    }
}

void MainWindow::openMyAnimeListImportDialog() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Nie jesteś zalogowany");
        return;
    }
    MyAnimeListDialog myAnimeListImportDialog(&dbManager);
    connect(&myAnimeListImportDialog, &MyAnimeListDialog::importSuccesfull, this, static_cast<void (MainWindow::*)()>(&MainWindow::loadSeries));

    myAnimeListImportDialog.exec();
}

void MainWindow::addSeries() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany, aby dodać serial");
        return;
    }

    QString title = ui->titleInput->text();
    QList<QListWidgetItem*> selectedGenres = ui->genreInput->selectedItems();
    QStringList genreList;
    for (auto& item : selectedGenres) {
        genreList.append(item->text());
    }
    QString genres = genreList.join(", ");
    QDate startDate = ui->startDateInput->date();
    QDate endDate = ui->endDateInput->date();
    int episodesWatched = ui->episodesWatchedInput->value();
    int allEpisodes = ui->allEpisodes->value();
    QString url = ui->urlInput->text();
    QString status = ui->statusInput->currentText();
    int grade = ui->ratingSlider->value();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić tytuł");
        return;
    }

    QString formattedStartDate = startDate.toString("dd-MM-yyyy");
    QString formattedEndDate = endDate.toString("dd-MM-yyyy");

    Series series(-1, title, genres, formattedStartDate, formattedEndDate, episodesWatched, allEpisodes, url, status, grade);

    if (dbManager.addSeries(series)){
        QMessageBox::information(this, "Sukces", "Serial został dodany");
        loadSeries();
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się dodać serialu");
    }
}

void MainWindow::loadSeries() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany, aby wyświetlić seriale.");
        return;
    }

    ui->seriesTable->setRowCount(0);
    ui->seriesTable->setColumnCount(11);
    ui->seriesTable->setHorizontalHeaderLabels({"Tytuł", "Gatunki", "Rozpoczęto", "Zakończono", "Obejrzane Odcinki", "Wszystkie Odcinki", "URL", "Status", "Ocena", "Zatwierdź Zmiany", "ID"});
    QVector<Series> seriesList = dbManager.getSeries();
    for (int i = 0; i < seriesList.size(); i++) {
        try {
            ui->seriesTable->insertRow(i);
            ui->seriesTable->setItem(i, 0, new QTableWidgetItem(seriesList.at(i).getTitle()));
            ui->seriesTable->setItem(i, 1, new QTableWidgetItem(seriesList.at(i).getGenre()));

            QString startDate = seriesList.at(i).getStartingDate();
            QTableWidgetItem* startItem = new QTableWidgetItem(startDate);
            if (Series::isDefaultDate(startDate)) {
                startItem->setText("Brak");
                startItem->setForeground(Qt::gray);
                startItem->setData(Qt::UserRole, true);
            } else {
                startItem->setData(Qt::UserRole, false);
            }
            ui->seriesTable->setItem(i, 2, startItem);

            QString endDate = seriesList.at(i).getEndingDate();
            QTableWidgetItem* endItem = new QTableWidgetItem(endDate);
            if (Series::isDefaultDate(endDate)) {
                endItem->setText("Brak");
                endItem->setForeground(Qt::gray);
                endItem->setData(Qt::UserRole, true);
            } else {
                endItem->setData(Qt::UserRole, false);
            }
            ui->seriesTable->setItem(i, 3, endItem);
            ui->seriesTable->setItem(i, 4, new QTableWidgetItem(seriesList.at(i).getEpisodesWatched()));
            ui->seriesTable->setItem(i, 5, new QTableWidgetItem(seriesList.at(i).getAllEpisodes()));
            ui->seriesTable->setItem(i, 6, new QTableWidgetItem(seriesList.at(i).getUrl()));
            ui->seriesTable->setItem(i, 7, new QTableWidgetItem(seriesList.at(i).getCategory()));
            ui->seriesTable->setItem(i, 8, new QTableWidgetItem(seriesList.at(i).getGrade()));

            QPushButton* acceptButton = new QPushButton("Zatwierdź", this);
            ui->seriesTable->setCellWidget(i, 9, acceptButton);
            connect(acceptButton, &QPushButton::clicked, this, &MainWindow::acceptChanges);

            QTableWidgetItem* idItem = new QTableWidgetItem();
            idItem->setData(Qt::UserRole, seriesList.at(i).getId());
            ui->seriesTable->setItem(i, 10, idItem);
            ui->seriesTable->setColumnHidden(10, true);

            connect(ui->seriesTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::removePlaceholderText);
        }  catch (const std::exception& e) {
            qDebug() << "Błąd podczas dodawania wiersza do tabeli: " << e.what();
        }
    }
}

void MainWindow::loadSeries(QVector<Series> seriesList) {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany, aby wyświetlić seriale.");
        return;
    }

    ui->seriesTable->setRowCount(0);
    ui->seriesTable->setColumnCount(11);
    ui->seriesTable->setHorizontalHeaderLabels({"Tytuł", "Gatunki", "Rozpoczęto", "Zakończono", "Obejrzane Odcinki", "Wszystkie Odcinki", "URL", "Status", "Ocena", "Zatwierdź Zmiany", "ID"});
    for (int i = 0; i < seriesList.size(); i++) {
        try {
            ui->seriesTable->insertRow(i);
            ui->seriesTable->setItem(i, 0, new QTableWidgetItem(seriesList.at(i).getTitle()));
            ui->seriesTable->setItem(i, 1, new QTableWidgetItem(seriesList.at(i).getGenre()));

            QString startDate = seriesList.at(i).getStartingDate();
            QTableWidgetItem* startItem = new QTableWidgetItem(startDate);
            if (Series::isDefaultDate(startDate)) {
                startItem->setText("Brak");
                startItem->setForeground(Qt::gray);
                startItem->setData(Qt::UserRole, true);
            } else {
                startItem->setData(Qt::UserRole, false);
            }
            ui->seriesTable->setItem(i, 2, startItem);

            QString endDate = seriesList.at(i).getEndingDate();
            QTableWidgetItem* endItem = new QTableWidgetItem(endDate);
            if (Series::isDefaultDate(endDate)) {
                endItem->setText("Brak");
                endItem->setForeground(Qt::gray);
                endItem->setData(Qt::UserRole, true);
            } else {
                endItem->setData(Qt::UserRole, false);
            }
            ui->seriesTable->setItem(i, 3, endItem);
            ui->seriesTable->setItem(i, 4, new QTableWidgetItem(seriesList.at(i).getEpisodesWatched()));
            ui->seriesTable->setItem(i, 5, new QTableWidgetItem(seriesList.at(i).getAllEpisodes()));
            ui->seriesTable->setItem(i, 6, new QTableWidgetItem(seriesList.at(i).getUrl()));
            ui->seriesTable->setItem(i, 7, new QTableWidgetItem(seriesList.at(i).getCategory()));
            ui->seriesTable->setItem(i, 8, new QTableWidgetItem(seriesList.at(i).getGrade()));

            QPushButton* acceptButton = new QPushButton("Zatwierdź", this);
            ui->seriesTable->setCellWidget(i, 9, acceptButton);
            connect(acceptButton, &QPushButton::clicked, this, &MainWindow::acceptChanges);

            QTableWidgetItem* idItem = new QTableWidgetItem();
            idItem->setData(Qt::UserRole, seriesList.at(i).getId());
            ui->seriesTable->setItem(i, 10, idItem);
            ui->seriesTable->setColumnHidden(10, true);

            connect(ui->seriesTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::removePlaceholderText);
        }  catch (const std::exception& e) {
            qDebug() << "Błąd podczas dodawania wiersza do tabeli: " << e.what();
        }
    }
}

void MainWindow::acceptChanges() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }

    int row = ui->seriesTable->indexAt(button->pos()).row();
    QTableWidgetItem* idItem = ui->seriesTable->item(row, 10);

    if (!idItem) {
        QMessageBox::warning(this, "Błąd", "Nie można znaleźć ID dla wybranej serii.");
        return;
    }

    int id = idItem->data(Qt::UserRole).toInt();
    QString newTitle = ui->seriesTable->item(row, 0)->text();
    QString newGenre = ui->seriesTable->item(row, 1)->text();
    QString newStartingDate = ui->seriesTable->item(row, 2)->text();
    QString newEndingDate = ui->seriesTable->item(row, 3)->text();
    int newEpisodesWatched = ui->seriesTable->item(row, 4)->text().toInt();
    int newAllEpisodes = ui->seriesTable->item(row, 5)->text().toInt();
    QString newUrl = ui->seriesTable->item(row, 6)->text();
    QString newCategory = ui->seriesTable->item(row, 7)->text();
    int newGrade = ui->seriesTable->item(row, 8)->text().toInt();

    Series previousSelectedSerie = dbManager.getSeriesById(id);
    Series updatedSerie(id, newTitle, newGenre, newStartingDate, newEndingDate, newEpisodesWatched, newAllEpisodes, newUrl, newCategory, newGrade);

    if (CheckSeriesEquality(previousSelectedSerie, updatedSerie) == true) {
        QMessageBox::warning(this, "Błąd", "Nie dokonano żadnej zmiany");
        return;
    }

    if (dbManager.updateSeries(updatedSerie)) {
        QMessageBox::information(this, "Sukces", "Pomyślnie zaktualizowano serial");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się zaktualizować serialu");
    }

}

bool MainWindow::CheckSeriesEquality(Series previousSeries, Series newSeries) {
    return previousSeries.getTitle() == newSeries.getTitle() &&
           previousSeries.getGenre() == newSeries.getGenre() &&
           previousSeries.getStartingDate() == newSeries.getStartingDate() &&
           previousSeries.getEndingDate() == newSeries.getEndingDate() &&
           previousSeries.getEpisodesWatched() == newSeries.getEpisodesWatched() &&
           previousSeries.getAllEpisodes() == newSeries.getAllEpisodes() &&
           previousSeries.getUrl() == newSeries.getUrl() &&
           previousSeries.getCategory() == newSeries.getCategory() &&
           previousSeries.getGrade() == newSeries.getGrade();
}


void MainWindow::updateUIBasedOnLoginStatus() {
    ui->addButton->setEnabled(isLoggedIn);
    ui->deleteButton->setEnabled(isLoggedIn);
    ui->seriesTable->setEnabled(isLoggedIn);

    if (isLoggedIn) {
        loadSeries();
    } else {
        ui->seriesTable->setRowCount(0);
    }
}

void MainWindow::removeSeries() {
    QModelIndexList selectedRows = ui->seriesTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć serial do usunięcia");
        return;
    }

    int row = selectedRows.at(0).row();
    QTableWidgetItem *item = ui->seriesTable->item(row, 10);

    if (!item) {
        QMessageBox::warning(this, "Błąd", "Nie można znaleźć wybranej serii.");
        return;
    }

    int id = item->data(Qt::UserRole).toInt();
    if (dbManager.deleteSeries(id)) {
        QMessageBox::information(this, "Sukces", "Serial został usunięty.");
        ui->seriesTable->removeRow(row);
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się usunąć serialu");
    }
}

void MainWindow::removePlaceholderText(int row, int column) {
    QTableWidgetItem* item = ui->seriesTable->item(row, column);
    if (!item) {
        return;
    }

    bool isPlaceholder = item->data(Qt::UserRole).toBool();

    if (isPlaceholder) {
        item->setText("");
        item->setForeground(Qt::black);
        item->setData(Qt::UserRole, false);
    }
}

void MainWindow::searchSeriesByTitle() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany");
        return;
    }

    QDialog* searchDialog = new QDialog(this);
    searchDialog->resize(300, 100);
    searchDialog->setWindowTitle("Wyszukiwanie seriali po tytule");

    QLabel* titleInfo = new QLabel(this);
    titleInfo->setText("Wprowadź tytuł");
    QLineEdit* titleLineEdit = new QLineEdit(searchDialog);
    QPushButton* searchButton = new QPushButton("Szukaj", searchDialog);

    QVBoxLayout* layout = new QVBoxLayout(searchDialog);
    layout->addWidget(titleInfo);
    layout->addWidget(titleLineEdit);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, [=]() {
       QString title = titleLineEdit->text().trimmed();
       QPair<bool, QVector<Series>> result = dbManager.getSeriesByTitle(title);

       if (result.first) {
           loadSeries(result.second);
       } else {
           QMessageBox::warning(this, "Błąd", "Nie udało się wyszukać seriali");
       }

       searchDialog->close();
    });

    searchDialog->exec();
}

void MainWindow::searchSeriesByGenre() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany");
        return;
    }

    QDialog* searchDialog = new QDialog(this);
    searchDialog->resize(300, 150);
    searchDialog->setWindowTitle("Wyszukiwanie seriali po gatunku");

    QComboBox* genreComboBox = new QComboBox(searchDialog);
    genreComboBox->addItem("Akcja");
    genreComboBox->addItem("Komedia");
    genreComboBox->addItem("Dramat");
    genreComboBox->addItem("Fantasy");
    genreComboBox->addItem("Romans");
    genreComboBox->addItem("Thriller");
    genreComboBox->addItem("Horror");
    genreComboBox->addItem("Przygodowe");
    genreComboBox->addItem("Sci-Fi");
    QPushButton* searchButton = new QPushButton("Szukaj", searchDialog);

    QVBoxLayout* layout = new QVBoxLayout(searchDialog);
    layout->addWidget(new QLabel("Wybierz gatunek", searchDialog));
    layout->addWidget(genreComboBox);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, [=]() {
       QString selectedGenre = genreComboBox->currentText();
       QPair<bool, QVector<Series>> result = dbManager.getSeriesByGenre(selectedGenre);

       if (result.first) {
           loadSeries(result.second);
       } else {
           QMessageBox::warning(this, "Błąd", "Nie udało się wyszukać seriali");
       }

       searchDialog->close();
    });

    searchDialog->exec();
}

void MainWindow::searchSeriesByCategory() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany");
        return;
    }

    QDialog* searchDialog = new QDialog(this);
    searchDialog->resize(300, 150);
    searchDialog->setWindowTitle("Wyszukiwanie seriali po statusie");

    QComboBox* statusComboBox = new QComboBox(searchDialog);
    statusComboBox->addItem("Oglądane");
    statusComboBox->addItem("Wstrzymane");
    statusComboBox->addItem("Zakończone");
    statusComboBox->addItem("Porzucone");
    statusComboBox->addItem("Planowane");
    QPushButton* searchButton = new QPushButton("Szukaj", searchDialog);

    QVBoxLayout* layout = new QVBoxLayout(searchDialog);
    layout->addWidget(new QLabel("Wybierz status", searchDialog));
    layout->addWidget(statusComboBox);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, [=]() {
       QString selectedStatus = statusComboBox->currentText();
       QPair<bool, QVector<Series>> result = dbManager.getSeriesByCategory(selectedStatus);

       if (result.first) {
           loadSeries(result.second);
       } else {
           QMessageBox::warning(this, "Błąd", "Nie udało się wyszukać seriali");
       }

       searchDialog->close();
    });

    searchDialog->exec();
}

void MainWindow::searchSeriesByGrade() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany");
        return;
    }

    QDialog* searchDialog = new QDialog(this);
    searchDialog->resize(300, 150);
    searchDialog->setWindowTitle("Wyszukiwanie seriali po ocenie");

    QComboBox* gradeComboBox = new QComboBox(searchDialog);
    gradeComboBox->addItem("1");
    gradeComboBox->addItem("2");
    gradeComboBox->addItem("3");
    gradeComboBox->addItem("4");
    gradeComboBox->addItem("5");
    gradeComboBox->addItem("6");
    gradeComboBox->addItem("7");
    gradeComboBox->addItem("8");
    gradeComboBox->addItem("9");
    gradeComboBox->addItem("10");
    QPushButton* searchButton = new QPushButton("Szukaj", searchDialog);

    QVBoxLayout* layout = new QVBoxLayout(searchDialog);
    layout->addWidget(new QLabel("Wybierz ocenę", searchDialog));
    layout->addWidget(gradeComboBox);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, [=]() {
       int selectedGrade = gradeComboBox->currentText().toInt();
       QPair<bool, QVector<Series>> result = dbManager.getSeriesByGrade(selectedGrade);

       if (result.first) {
           loadSeries(result.second);
       } else {
           QMessageBox::warning(this, "Błąd", "Nie udało się wyszukać seriali");
       }

       searchDialog->close();
    });

    searchDialog->exec();
}

void MainWindow::loadAllSeries() {
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Błąd", "Musisz być zalogowany");
        return;
    }
    loadSeries();
}

MainWindow::~MainWindow()
{
    delete ui;
}


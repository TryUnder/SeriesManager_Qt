#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountDialog.h"
#include "LoginAccountDialog.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbManager("D:\\P.R.I.V\\Uczelnia\\Semestr_VIII\\Programowanie_Wieloplatformowe\\Projekt_Seriale_Db.db")
    , isLoggedIn(false)
{
    ui->setupUi(this);
    ui->genreInput->setMaximumWidth(75);
    ui->genreInput->setMaximumHeight(50);
    connect(ui->createAccountAction, &QAction::triggered, this, &MainWindow::createAccount);
    connect(ui->loginAccountAction, &QAction::triggered, this, &MainWindow::loginAccount);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addSeries);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::removeSeries);


    updateUIBasedOnLoginStatus();
}

void MainWindow::createAccount() {
    if (!dbManager.doesUserExist()) {
        CreateAccountDialog createAccountDialog(this);
        createAccountDialog.exec();
    } else {
        QMessageBox::information(this, "Informacja", "Konto już istnieje. Zaloguj się.");
    }
}

void MainWindow::loginAccount() {
    LoginAccountDialog loginAccountDialog(this);

    if (isLoggedIn == true) {
        return;
    }

    if (loginAccountDialog.exec() == QDialog::Accepted) {
        isLoggedIn = true;
        QMessageBox::information(this, "Sukces", "Zalogowano pomyślnie");
        updateUIBasedOnLoginStatus();
    }
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
    QString url = ui->urlInput->text();
    QString status = ui->statusInput->currentText();
    int grade = ui->ratingSlider->value();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić tytuł");
        return;
    }

    Series series(-1, title, genres, startDate, endDate, episodesWatched, url, status, grade);

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
    qDebug() << "JEST 4 ";
    ui->seriesTable->setRowCount(0); // Clear existing rows
    ui->seriesTable->setColumnCount(9); // Including hidden ID column
    ui->seriesTable->setHorizontalHeaderLabels({"Title", "Genre", "Starting Date", "Ending Date", "Episodes Watched", "URL", "Category", "Grade", "ID"});
    QVector<Series> seriesList = dbManager.getSeries();
    for (int i = 0; i < seriesList.size(); i++) {
        try {
            ui->seriesTable->insertRow(i);
            ui->seriesTable->setItem(i, 0, new QTableWidgetItem(seriesList.at(i).getTitle()));
            ui->seriesTable->setItem(i, 1, new QTableWidgetItem(seriesList.at(i).getGenre()));
            ui->seriesTable->setItem(i, 2, new QTableWidgetItem(seriesList.at(i).getStartingDate().toString()));
            ui->seriesTable->setItem(i, 3, new QTableWidgetItem(seriesList.at(i).getEndingDate().toString()));
            ui->seriesTable->setItem(i, 4, new QTableWidgetItem(seriesList.at(i).getEpisodesWatched()));
            ui->seriesTable->setItem(i, 5, new QTableWidgetItem(seriesList.at(i).getUrl()));
            ui->seriesTable->setItem(i, 6, new QTableWidgetItem(seriesList.at(i).getCategory()));
            ui->seriesTable->setItem(i, 7, new QTableWidgetItem(seriesList.at(i).getGrade()));

            QTableWidgetItem* idItem = new QTableWidgetItem();
            idItem->setData(Qt::UserRole, seriesList.at(i).getId());
            ui->seriesTable->setItem(i, 8, idItem);
            ui->seriesTable->setColumnHidden(8, true);
        }  catch (const std::exception& e) {
            qDebug() << "Error, inserting row in table: " << e.what();
        }
    }

   connect(ui->seriesTable, &QTableWidget::itemChanged, this, &MainWindow::updateSeries);
}

void MainWindow::updateUIBasedOnLoginStatus() {
    ui->addButton->setEnabled(isLoggedIn);
    ui->deleteButton->setEnabled(isLoggedIn);
    ui->seriesTable->setEnabled(isLoggedIn);

    if (isLoggedIn) {
        loadSeries();
    }
}

void MainWindow::removeSeries() {
    QModelIndexList selectedRows = ui->seriesTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć serial do usunięcia");
        return;
    }

    int row = selectedRows.at(0).row();
    QTableWidgetItem *item = ui->seriesTable->item(row, 8);

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

void MainWindow::updateSeries(QTableWidgetItem* item) {
    if (!item) {
        return;
    }

    int row = item->row();

    QTableWidgetItem* idItem = ui->seriesTable->item(row, 8);
    int id = idItem->data(Qt::UserRole).toInt();

    QString newTitle = ui->seriesTable->item(row, 0)->text();
    QString newGenre = ui->seriesTable->item(row, 1)->text();
    QDate newStartingDate = QDate::fromString(ui->seriesTable->item(row, 2)->text(), "yyyy-MM-dd");
    QDate newEndingDate = QDate::fromString(ui->seriesTable->item(row, 3)->text(), "yyyy-MM-dd");
    int newEpisodesWatched = ui->seriesTable->item(row, 4)->text().toInt();
    QString newUrl = ui->seriesTable->item(row, 5)->text();
    QString newCategory = ui->seriesTable->item(row, 6)->text();
    int newGrade = ui->seriesTable->item(row, 7)->text().toInt();

    Series existingSeries = dbManager.getSeriesById(id); // Implement this function to retrieve series by id

        if (newTitle == existingSeries.getTitle() &&
            newGenre == existingSeries.getGenre() &&
            newStartingDate == existingSeries.getStartingDate() &&
            newEndingDate == existingSeries.getEndingDate() &&
            newEpisodesWatched == existingSeries.getEpisodesWatched().toInt() &&
            newUrl == existingSeries.getUrl() &&
            newCategory == existingSeries.getCategory() &&
            newGrade == existingSeries.getGrade().toInt()) {
            // No changes detected, return early
            return;
        }

    Series updatedSeries(id, newTitle, newGenre, newStartingDate, newEndingDate, newEpisodesWatched, newUrl, newCategory, newGrade);
    if (dbManager.updateSeries(updatedSeries)) {
        QMessageBox::information(this, "Sukces", "Serial został zaktualizowany");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się zaktualizować serialu");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


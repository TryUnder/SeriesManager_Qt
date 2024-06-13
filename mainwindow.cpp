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

    QString formattedStartDate = startDate.toString("dd-MM-yyyy");
    QString formattedEndDate = endDate.toString("dd-MM-yyyy");

    Series series(-1, title, genres, formattedStartDate, formattedEndDate, episodesWatched, url, status, grade);

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
    ui->seriesTable->setColumnCount(10);
    ui->seriesTable->setHorizontalHeaderLabels({"Tytuł", "Gatunki", "Rozpoczęto", "Zakończono", "Obejrzane Odcinki", "URL", "Status", "Ocena", "Zatwierdź Zmiany", "ID"});
    QVector<Series> seriesList = dbManager.getSeries();
    for (int i = 0; i < seriesList.size(); i++) {
        try {
            ui->seriesTable->insertRow(i);
            ui->seriesTable->setItem(i, 0, new QTableWidgetItem(seriesList.at(i).getTitle()));
            ui->seriesTable->setItem(i, 1, new QTableWidgetItem(seriesList.at(i).getGenre()));
            ui->seriesTable->setItem(i, 2, new QTableWidgetItem(seriesList.at(i).getStartingDate()));
            ui->seriesTable->setItem(i, 3, new QTableWidgetItem(seriesList.at(i).getEndingDate()));
            ui->seriesTable->setItem(i, 4, new QTableWidgetItem(seriesList.at(i).getEpisodesWatched()));
            ui->seriesTable->setItem(i, 5, new QTableWidgetItem(seriesList.at(i).getUrl()));
            ui->seriesTable->setItem(i, 6, new QTableWidgetItem(seriesList.at(i).getCategory()));
            ui->seriesTable->setItem(i, 7, new QTableWidgetItem(seriesList.at(i).getGrade()));

            QPushButton* acceptButton = new QPushButton("Zatwierdź", this);
            ui->seriesTable->setCellWidget(i, 8, acceptButton);
            connect(acceptButton, &QPushButton::clicked, this, &MainWindow::acceptChanges);

            QTableWidgetItem* idItem = new QTableWidgetItem();
            idItem->setData(Qt::UserRole, seriesList.at(i).getId());
            ui->seriesTable->setItem(i, 9, idItem);
            ui->seriesTable->setColumnHidden(9, true);
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
    QTableWidgetItem* idItem = ui->seriesTable->item(row, 9);

    if (!idItem) {
        QMessageBox::warning(this, "Błąd", "Nie można znaleźć ID dla wybranej serii.");
        return;
    }

    qDebug() << "row: " << row << " id: " << idItem->data(Qt::UserRole).toInt();

    int id = idItem->data(Qt::UserRole).toInt();
    QString newTitle = ui->seriesTable->item(row, 0)->text();
    QString newGenre = ui->seriesTable->item(row, 1)->text();
    QString newStartingDate = ui->seriesTable->item(row, 2)->text();
    QString newEndingDate = ui->seriesTable->item(row, 3)->text();
    int newEpisodesWatched = ui->seriesTable->item(row, 4)->text().toInt();
    QString newUrl = ui->seriesTable->item(row, 5)->text();
    QString newCategory = ui->seriesTable->item(row, 6)->text();
    int newGrade = ui->seriesTable->item(row, 7)->text().toInt();

    Series previousSelectedSerie = dbManager.getSeriesById(id);
    Series updatedSerie(id, newTitle, newGenre, newStartingDate, newEndingDate, newEpisodesWatched, newUrl, newCategory, newGrade);

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
    }
}

void MainWindow::removeSeries() {
    QModelIndexList selectedRows = ui->seriesTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę zaznaczyć serial do usunięcia");
        return;
    }

    int row = selectedRows.at(0).row();
    QTableWidgetItem *item = ui->seriesTable->item(row, 9);

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

MainWindow::~MainWindow()
{
    delete ui;
}


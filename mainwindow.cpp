#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createaccountDialog.h"
#include "LoginAccountDialog.h"
#include <QMessageBox>

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

    QVector<Series> seriesList = dbManager.getSeries();
    for (auto& serie : seriesList) {
        int id = serie.getId() - 1;
        ui->seriesTable->insertRow(id);
        ui->seriesTable->setItem(id, 0, new QTableWidgetItem(serie.getTitle()));
        ui->seriesTable->setItem(id, 1, new QTableWidgetItem(serie.getGenre()));
        ui->seriesTable->setItem(id, 2, new QTableWidgetItem(serie.getStartingDate().toString()));
        ui->seriesTable->setItem(id, 3, new QTableWidgetItem(serie.getEndingDate().toString()));
        ui->seriesTable->setItem(id, 4, new QTableWidgetItem(serie.getEpisodesWatched()));
        ui->seriesTable->setItem(id, 5, new QTableWidgetItem(serie.getUrl()));
        ui->seriesTable->setItem(id, 6, new QTableWidgetItem(serie.getCategory()));
        ui->seriesTable->setItem(id, 7, new QTableWidgetItem(serie.getGrade()));
    }
}

void MainWindow::updateUIBasedOnLoginStatus() {
    ui->addButton->setEnabled(isLoggedIn);
    ui->updateButton->setEnabled(isLoggedIn);
    ui->seriesTable->setEnabled(isLoggedIn);

    if (isLoggedIn) {
        loadSeries();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


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
    QString genre = ui->genreInput->currentText();
    QDate startDate = ui->startDateInput->date();
    QDate endDate = ui->endDateInput->date();
    int episodesWatched = ui->episodesWatchedInput->value();
    QString url = ui->urlInput->text();
    QString status = ui->statusInput->currentText();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Błąd", "Proszę wypełnić tytuł");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Series (title, genre, starting_date, ending_date, episodes_watched, url, category) "
                  "VALUES (:title, :genre, :starting_date, :ending_date, :episodes_watched, :url, :category)");
    query.bindValue(":title", title);
    query.bindValue(":genre", genre);
    query.bindValue(":starting_date", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":ending_date", endDate.toString("yyyy-MM-dd"));
    query.bindValue(":episodes_watched", episodesWatched);
    query.bindValue(":url", url);
    query.bindValue(":category", status);

    if (query.exec()) {
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

    QSqlQuery query("SELECT title, genre, starting_date, ending_date, episodes_watched, url, category FROM Series");

    ui->seriesTable->setRowCount(0);

    int row = 0;
    while(query.next()) {
        ui->seriesTable->insertRow(row);
        ui->seriesTable->setItem(row, 0, new QTableWidgetItem(query.value("title").toString()));
        ui->seriesTable->setItem(row, 1, new QTableWidgetItem(query.value("genre").toString()));
        ui->seriesTable->setItem(row, 2, new QTableWidgetItem(query.value("starting_date").toString()));
        ui->seriesTable->setItem(row, 3, new QTableWidgetItem(query.value("ending_date").toString()));
        ui->seriesTable->setItem(row, 4, new QTableWidgetItem(query.value("episodes_watched").toString()));
        ui->seriesTable->setItem(row, 5, new QTableWidgetItem(query.value("url").toString()));
        ui->seriesTable->setItem(row, 6, new QTableWidgetItem(query.value("category").toString()));
        row++;
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


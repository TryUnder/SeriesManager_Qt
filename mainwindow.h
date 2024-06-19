#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "databasemanager.h"
#include "accountmanager.h"
#include "remindpassword.h"
#include "deleteaccount.h"
#include "myanimelistdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool CheckSeriesEquality(Series previousSeries, Series newSeries);

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    bool isLoggedIn;
    void updateUIBasedOnLoginStatus();
    accountmanager accountManager;

private slots:
    void createAccount();
    void loginAccount();
    void addSeries();
    void loadSeries();
    void removeSeries();
    void acceptChanges();
    void remindPassword();
    void logoutUser();
    void deleteAccount();
    void openMyAnimeListImportDialog();
    void removePlaceholderText(int row, int column);
};
#endif // MAINWINDOW_H

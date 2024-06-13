#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "databasemanager.h"

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

private slots:
    void createAccount();
    void loginAccount();
    void addSeries();
    void loadSeries();
    void removeSeries();
    void acceptChanges();
};
#endif // MAINWINDOW_H

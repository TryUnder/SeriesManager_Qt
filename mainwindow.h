#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
};
#endif // MAINWINDOW_H

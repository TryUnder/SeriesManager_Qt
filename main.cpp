#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qDebug() << "Drivers: " << QSqlDatabase::drivers();
    DatabaseManager dbManager("localhost", "root", "ProjectScrabble", "projekt_qt");

    w.show();
    return a.exec();
}

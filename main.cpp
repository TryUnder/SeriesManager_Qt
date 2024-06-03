#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    qDebug() << "Drivers: " << QSqlDatabase::drivers();
    DatabaseManager dbManager("D:\\P.R.I.V\\Uczelnia\\Semestr_VIII\\Programowanie_Wieloplatformowe\\Projekt_Seriale_Db.db");

    w.show();
    return a.exec();
}

#ifndef WATCHMANAGER_H
#define WATCHMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QVideoWidget>
#include <QMainWindow>
#include <QPointer>
#include "databasemanager.h"
#include "mainwindow.h"

class WatchManager : public QMainWindow
{
    Q_OBJECT
public:
    WatchManager(DatabaseManager* dbManager, QWidget* parent = nullptr);
    void watchSeries();

private slots:
    void selectEpisode(const QString& discLocation);
    void playEpisode(const QString& episodePath);

private:

    DatabaseManager* m_dbManager;
    QWidget* originalCentralWidget = nullptr;
};

#endif // WATCHMANAGER_H

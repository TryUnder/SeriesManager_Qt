#ifndef MYANIMELISTDIALOG_H
#define MYANIMELISTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include "databasemanager.h"
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QFile>
#include "series.h"

namespace Ui {
class MyAnimeListDialog;
}

class MyAnimeListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyAnimeListDialog(DatabaseManager* dbManager, QDialog *parent = nullptr);
    ~MyAnimeListDialog();
    void setMyAnimeListXmlPath(QString myAnimeListXmlPath);
    QString getMyAnimeListXmlPath();
    void onBrowseButtonClicked();
    void onImportButtonClicked();
    bool importFromXML();

private:
    Ui::MyAnimeListDialog *ui;
    QString m_myAnimeListXmlPath;
    DatabaseManager* m_dbManager;
};

#endif // MYANIMELISTDIALOG_H

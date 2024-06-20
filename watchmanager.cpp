#include "watchmanager.h"

WatchManager::WatchManager(DatabaseManager* dbManager, QWidget* parent) :
    QObject(parent),
    m_dbManager(dbManager),
    parentWidget(parent) { }

void WatchManager::watchSeries() {
    QVector<Series> result = m_dbManager->getSeries();

    if (result.isEmpty())  {
        QMessageBox::warning(parentWidget, "Błąd", "Nie znaleziono dostępnych seriali");
        return;
    }

    QDialog* seriesDialog = new QDialog(parentWidget);
    seriesDialog->setWindowTitle("Wybierz tytuł");
    QVBoxLayout* layout = new QVBoxLayout(seriesDialog);

    QComboBox* seriesComboBox = new QComboBox(seriesDialog);
    for (const Series &series : result) {
        seriesComboBox->addItem(series.getTitle(), series.getDiscLocation());
    }

    QPushButton* selectButton = new QPushButton("Wybierz", seriesDialog);
    layout->addWidget(new QLabel("Wybierz serial:", seriesDialog));
    layout->addWidget(seriesComboBox);
    layout->addWidget(selectButton);

    connect(selectButton, &QPushButton::clicked, [=]() {
        QString discLocation = seriesComboBox->currentData().toString();
        seriesDialog->close();
        selectEpisode(discLocation);
    });

    seriesDialog->exec();
}

void WatchManager::selectEpisode(const QString& discLocation) {
    QString episodePath = QFileDialog::getOpenFileName(parentWidget, "Wybierz odcinek", discLocation, "Video Files (*.mkv *.mp4 *.avi)");
    if (!episodePath.isEmpty()) {
        playEpisode(episodePath);
    }
}

void WatchManager::playEpisode(const QString& episodePath) {
    if (!parentWidget) {
        return;
    }

    originalCentralWidget = static_cast<QMainWindow*>(parentWidget)->centralWidget();

    QMediaPlayer* player = new QMediaPlayer(parentWidget);
    QVideoWidget* videoWidget = new QVideoWidget(parentWidget);

    player->setVideoOutput(videoWidget);

    QPushButton* playButton = new QPushButton("Play");
    QPushButton* pauseButton = new QPushButton("Pause");
    QPushButton* stopButton = new QPushButton("Stop");
    QPushButton* closeButton = new QPushButton("Close");
    QSlider* positionSlider = new QSlider(Qt::Horizontal);

    QHBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(closeButton);
    controlLayout->addWidget(positionSlider);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);

    QWidget* centralWidget = new QWidget(parentWidget);
    centralWidget->setLayout(layout);
    static_cast<QMainWindow*>(parentWidget)->setCentralWidget(centralWidget);

    QPointer<QWidget> centralWidgetPtr = centralWidget;  // Ensure <QPointer> is included

    QUrl discLocationUrl = QUrl::fromLocalFile(episodePath);
    player->setMedia(discLocationUrl);
    connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
    connect(stopButton, &QPushButton::clicked, player, &QMediaPlayer::stop);
    connect(closeButton, &QPushButton::clicked, [=]() {
        player->stop();
        restoreOriginalCentralWidget();
    });

    connect(player, &QMediaPlayer::positionChanged, positionSlider, &QSlider::setValue);
    connect(player, &QMediaPlayer::durationChanged, positionSlider, &QSlider::setMaximum);
    connect(positionSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    player->play();
}

void WatchManager::restoreOriginalCentralWidget() {
    if (originalCentralWidget) {
        static_cast<QMainWindow*>(parentWidget)->setCentralWidget(originalCentralWidget);
        originalCentralWidget = nullptr; // Clear the pointer after restoring the original central widget

        // Call loadSeries from MainWindow to reload the series list
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget);
        if (mainWindow) {
            QVector<Series> seriesList = m_dbManager->getSeries();
            mainWindow->loadSeries(seriesList);
        }
    }
}

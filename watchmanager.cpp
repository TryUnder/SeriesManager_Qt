#include "watchmanager.h"

WatchManager::WatchManager(DatabaseManager* dbManager, QWidget* parent) :
    QMainWindow(parent),
    m_dbManager(dbManager) {

    setWindowTitle("Obejrzyj Serial");
    setAttribute(Qt::WA_DeleteOnClose);
    resize(800, 600);
    watchSeries();
}

void WatchManager::watchSeries() {
    QVector<Series> result = m_dbManager->getSeries();

    if (result.isEmpty())  {
        QMessageBox::warning(this, "Błąd", "Nie znaleziono dostępnych seriali");
        close();
        return;
    }

    QDialog* seriesDialog = new QDialog(this);
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
        if (!discLocation.isEmpty()) {
            seriesDialog->accept();
            selectEpisode(discLocation);
        } else {
            QMessageBox::warning(this, "Błąd", "Serial nie ma wprowadzonej ścieżki do folderu");
            seriesDialog->reject();
        }
    });

    connect(seriesDialog, &QDialog::rejected, this, &WatchManager::close);

    seriesDialog->exec();
}

void WatchManager::selectEpisode(const QString& discLocation) {
    QString episodePath = QFileDialog::getOpenFileName(this, "Wybierz odcinek", discLocation, "Video Files (*.mkv *.mp4 *.avi)");
    if (!episodePath.isEmpty()) {
        playEpisode(episodePath);
    } else {
        QMessageBox::warning(this, "Błąd", "Wybierz odcinek do odtworzenia");
        close();
    }
}

void WatchManager::playEpisode(const QString& episodePath) {
    QMediaPlayer* player = new QMediaPlayer(this);
    QVideoWidget* videoWidget = new QVideoWidget(this);

    player->setVideoOutput(videoWidget);

    QPushButton* playButton = new QPushButton("Play");
    QPushButton* pauseButton = new QPushButton("Pause");
    QPushButton* stopButton = new QPushButton("Stop");
    QPushButton* closeButton = new QPushButton("Close");
    QSlider* positionSlider = new QSlider(Qt::Horizontal);

    playButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    pauseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    stopButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    closeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    positionSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QHBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(closeButton);
    controlLayout->addWidget(positionSlider);

    controlLayout->setStretch(0, 1);
    controlLayout->setStretch(1, 1);
    controlLayout->setStretch(2, 1);
    controlLayout->setStretch(3, 1);
    controlLayout->setStretch(4, 3);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(videoWidget);
    layout->addLayout(controlLayout);

    layout->setStretch(0, 8);
    layout->setStretch(1, 1);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QUrl discLocationUrl = QUrl::fromLocalFile(episodePath);
    player->setMedia(discLocationUrl);
    connect(playButton, &QPushButton::clicked, player, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, player, &QMediaPlayer::pause);
    connect(stopButton, &QPushButton::clicked, player, &QMediaPlayer::stop);
    connect(closeButton, &QPushButton::clicked, [=]() {
        player->stop();
        close();
    });

    connect(player, &QMediaPlayer::positionChanged, positionSlider, &QSlider::setValue);
    connect(player, &QMediaPlayer::durationChanged, positionSlider, &QSlider::setMaximum);
    connect(positionSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);

    player->play();
}

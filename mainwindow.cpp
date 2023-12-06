#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QCoreApplication>

// MainWindow constructor initializes the main window and its components.
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mediaPlayer(new QMediaPlayer(this)),
    videoWidget(new QVideoWidget(this))
{
    // Set up the user interface
    ui->setupUi(this);

    // Add the video widget to the vertical layout of the main window
    ui->verticalLayout->addWidget(videoWidget);

    // Set the video output of the media player to the video widget
    mediaPlayer->setVideoOutput(videoWidget);

    // Connect signals and slots for media playback control
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgressBar);
    connect(ui->progressbar, &QSlider::sliderMoved, this, &MainWindow::onProgressbarSliderMoved);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// onPauseClicked() toggles the play/pause state of the media player.
// If the media player is currently playing, it pauses playback;
// otherwise, it starts or resumes playback.
void MainWindow::onPauseClicked()
{
    // Toggle play/pause state
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        // If the media player is currently in the playing state, pause playback
        mediaPlayer->pause();
    } else {
        // If the media player is not in the playing state, start or resume playback
        mediaPlayer->play();
    }
}

// onForwardClicked() fast-forwards the media playback by 5 seconds.
void MainWindow::onForwardClicked()
{
    // Fast forward by 5 seconds
    qint64 currentPosition = mediaPlayer->position();
    qint64 newPosition = currentPosition + 5000;
    mediaPlayer->setPosition(newPosition);
}

// onRetreatClicked() rewinds the media playback by 5 seconds.
void MainWindow::onRetreatClicked()
{
    // Rewind by 5 seconds
    qint64 currentPosition = mediaPlayer->position();
    qint64 newPosition = currentPosition - 5000;
    mediaPlayer->setPosition(newPosition);
}

// onProgressbarSliderMoved() handles the sliderMoved signal of the progress bar.
// It sets the media player's position to the specified value, allowing the user
// to manually adjust the playback position by moving the progress bar slider.
// Params:
// - position: The new position set by the user through the progress bar slider.
void MainWindow::onProgressbarSliderMoved(int position)
{
    // Set the position moved by the slider to the media player
    mediaPlayer->setPosition(position);
}

// updateProgressBar() updates the progress bar based on the current position of the media playback.
void MainWindow::updateProgressBar(qint64 position)
{
    // Update the current value of the progress bar
    qint64 totalDuration = mediaPlayer->duration();

    if (totalDuration > 0) {
        // Set the range of the progress bar to the total duration of the media
        ui->progressbar->setRange(0, totalDuration);

        // Set the current value of the progress bar to the current position of the media playback
        ui->progressbar->setValue(position);
    }
}


// setFolderPath() sets the media player to play the selected video file.
// It opens a file dialog to choose a video file, sets the media player's media to the selected file,
// and initiates playback after waiting for the media to load.
// Params:
// - path: The initial path used by the file dialog.
void MainWindow::setFolderPath(const QString &path)
{
    // Log the folder path for debugging purposes
    qDebug() << "setFolderPath called with path:" << path;

    // Open the file dialog to choose a video file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose video file"), path, tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));

    // Log the selected file path for debugging purposes
    qDebug() << "Selected file path:" << filePath;

    // If the file path is not empty, set the media of the media player to the selected file and play
    if (!filePath.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));

        // Wait for the media to load before starting playback
        while (mediaPlayer->mediaStatus() != QMediaPlayer::LoadedMedia) {
            QCoreApplication::processEvents();
        }

        mediaPlayer->play();
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mediaPlayer(new QMediaPlayer(this)),
    videoWidget(new QVideoWidget(this))
{
    ui->setupUi(this);

    // Add videoWidget to the layout
    ui->verticalLayout->addWidget(videoWidget);

    // Set the video output window to videoWidget
    mediaPlayer->setVideoOutput(videoWidget);

    // Connect signals and slots
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgressBar);
    connect(ui->progressbar, &QSlider::sliderMoved, this, &MainWindow::on_progressbar_sliderMoved);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pause_clicked()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    } else {
        mediaPlayer->play();
    }
}

void MainWindow::on_forward_clicked()
{
    // Get the current position of the media
    qint64 currentPosition = mediaPlayer->position();

    // Set the forward time, here set to 5 seconds
    qint64 newPosition = currentPosition + 5000;

    // Set the new position to the media player
    mediaPlayer->setPosition(newPosition);
}

void MainWindow::on_retreat_clicked()
{
    // Get the current position of the media
    qint64 currentPosition = mediaPlayer->position();

    // Set the retreat time, here set to 5 seconds
    qint64 newPosition = currentPosition - 5000;

    // Set the new position to the media player
    mediaPlayer->setPosition(newPosition);
}

void MainWindow::on_progressbar_sliderMoved(int position)
{
    // Set the position moved by the slider to the media player
    mediaPlayer->setPosition(position);
}

void MainWindow::updateProgressBar(qint64 position)
{
    // Get the total duration of the current media
    qint64 totalDuration = mediaPlayer->duration();

    // Ensure that the total duration is not zero
    if (totalDuration > 0) {
        // Set QSlider's range to [0, totalDuration]
        ui->progressbar->setRange(0, totalDuration);

        // Set QSlider's current value to the current position of the media
        ui->progressbar->setValue(position);
    }
}


void MainWindow::setFolderPath(const QString &path)
{
    qDebug() << "setFolderPath called with path:" << path;

    // Handle the folder path here, you can store it as a member variable or use it directly
    // For example, you can use QFileDialog::getOpenFileName here to choose a file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose video file"), path, tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));

    // Output the file path
    qDebug() << "Selected file path:" << filePath;

    // If the user selects a file, set it as the video source
    if (!filePath.isEmpty()) {
        mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));

        // Wait for the media to be ready for playback
        while (mediaPlayer->mediaStatus() != QMediaPlayer::LoadedMedia) {
            QCoreApplication::processEvents();
        }

        mediaPlayer->play();
    }
}


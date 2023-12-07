#include "mainwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "btnconvert.h"
#include "ui_mainwindow.h"

// MainWindow constructor initializes the main window and its components.
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mediaPlayer(new QMediaPlayer(this)),
      videoWidget(new QVideoWidget(this)) {
    // Set up the user interface
    ui->setupUi(this);

    // Add the video widget to the vertical layout of the main window
    ui->verticalLayout->addWidget(videoWidget);

    // Add the list button to horizontal layout
    listsBtnsLayout = ui->lists->findChild<QHBoxLayout*>("horizontalLayout");

    // Render all lists
    fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
    listInfos_ = fileUtil_->GetAllListsInfo();

    for (size_t i = 0; i < listInfos_.size(); i++) {
        QPushButton* newButton = new QPushButton();
        newButton->setText(listInfos_[i].name.c_str());
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);

        listsBtnsLayout->addWidget(newButton);

        // connect onClick hook
        connect(newButton, &QPushButton::clicked, [this, i] { parseFolder(listInfos_[i].videoDirPath.c_str()); });
    }

    // Set the video output of the media player to the video widget
    mediaPlayer->setVideoOutput(videoWidget);

    // Connect signals and slots for media playback control
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgressBar);
    connect(ui->progressbar, &QSlider::sliderMoved, this, &MainWindow::onProgressbarSliderMoved);
}

MainWindow::~MainWindow() {
    delete ui;
}

// onPauseClicked() toggles the play/pause state of the media player.
// If the media player is currently playing, it pauses playback;
// otherwise, it starts or resumes playback.
void MainWindow::onPauseClicked() {
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
void MainWindow::onForwardClicked() {
    // Fast forward by 5 seconds
    qint64 currentPosition = mediaPlayer->position();
    qint64 newPosition = currentPosition + 5000;
    mediaPlayer->setPosition(newPosition);
}

// onRetreatClicked() rewinds the media playback by 5 seconds.
void MainWindow::onRetreatClicked() {
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
void MainWindow::onProgressbarSliderMoved(int position) {
    // Set the position moved by the slider to the media player
    mediaPlayer->setPosition(position);
}

// updateProgressBar() updates the progress bar based on the current position of the media playback.
void MainWindow::updateProgressBar(qint64 position) {
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
void MainWindow::setFolderPath(const QString& path) {
    // Log the folder path for debugging purposes
    qDebug() << "setFolderPath called with path:" << path;

    // Open the file dialog to choose a video file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Choose video file"), path,
                                                    tr("Video files (*.mp4 *.avi *.mkv);;All files (*)"));

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

void MainWindow::parseFolder(const QString& folderPath) {
    QDir dir(folderPath);
    QStringList videoFiles = dir.entryList(QStringList() << "*.mp4", QDir::Files);

    foreach (const QString& videoFile, videoFiles) {
        QString videoPath = dir.filePath(videoFile);
        videoPaths.append(videoPath);

        QString baseName = QFileInfo(videoFile).baseName();
        QString imagePath = dir.filePath(baseName + ".png");

        if (QFileInfo::exists(imagePath)) {
            BtnConvert* button = new BtnConvert(videoPath);
            button->setIcon(QIcon(imagePath));
            button->setIconSize(QSize(100, 100));
            connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

            QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->picturelist->layout());
            if (layout) {
                layout->addWidget(button);
            }
        }
    }
}

void MainWindow::onButtonClicked() {
    BtnConvert* button = qobject_cast<BtnConvert*>(sender());
    if (button) {
        QString videoPath = button->getVideoPath();
        int index = videoPaths.indexOf(videoPath);
        if (index != -1) {
            handleVideoSelection(videoPaths, index);
        }
    }
}

void MainWindow::handleVideoSelection(const QStringList& videoPaths, int currentIndex) {
    // 将视频路径数组和索引传递给接口
    // 这里实现接口调用的逻辑
    qDebug() << "Button clicked. Video paths:" << videoPaths;  // VideoPaths是一个字符串数组，存储文件夹里所有的视频路径
    qDebug() << "Button clicked. currentIndex:" << currentIndex;  // currentIndex是一个整型，显示当前按钮对应的索引
}

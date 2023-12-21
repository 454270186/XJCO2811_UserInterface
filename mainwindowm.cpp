#include "mainwindowm.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QVBoxLayout>
#include "btnconvert.h"
#include "listset.h"
#include "ui_mainwindowm.h"

#include <iostream>

// MainWindow constructor initializes the main window and its components.
mainwindowm::mainwindowm(QWidget* parent, MainWindowResource* cr)
    : QMainWindow(parent), ui(new Ui::mainwindowm), videoWidget(new QVideoWidget(this)) {
    // Init common resource
    commonResrc = cr;

    // Set up the user interface
    ui->setupUi(this);

    ui->lists->setStyleSheet("QScrollArea { border: 0; }");
    ui->fullandmoblie->setStyleSheet("QGroupBox { border: 0; }");
    ui->videoBox->setStyleSheet("QGroupBox { border: 0; }");

    // Set the minimum size to 460x700
    setMinimumSize(460, 700);

    // Assuming that "videoplayer" is the name of the QWidget in your UI file
    QWidget* videoplayer = ui->videoplayer;

    // Set up layout for videoWidget
    QVBoxLayout* videoLayout = new QVBoxLayout(videoplayer);
    videoLayout->addWidget(videoWidget);

    // 设置初始音量为 50
    commonResrc->mediaPlayer_->setVolume(50);

    // 将 QSlider 初始值设置为 50
    ui->voicecontrolstrip->setValue(50);

    ui->videoBox->installEventFilter(this);

    ui->voicecontrolstrip->hide();

    isVideoPlaying = false;
    ui->video->hide();

    // Assuming ui->lists is now a QScrollArea
    QScrollArea* listsScrollArea = ui->lists;

    // Create a QWidget to serve as the container for the buttons
    QWidget* listsContainer = new QWidget(listsScrollArea);

    // Create a QHBoxLayout for the buttons
    listsBtnsLayout = new QHBoxLayout(listsContainer);

    // Render all lists
    //    fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
    //    listInfos_ = fileUtil_->GetAllListsInfo();

    // Clear existing buttons
    QLayoutItem* child;
    while ((child = listsBtnsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Create new buttons based on updated listInfos_
    //    for (size_t i = 0; i < commonResrc->listinfo_.size(); i++) {
    //        QPushButton* newButton = new QPushButton();
    //        newButton->setText(commonResrc->listinfo_[i].name.c_str());
    //        newButton->setCheckable(true);
    //        newButton->setAutoExclusive(true);

    //        // 设置按钮的最小和最大大小
    //        newButton->setMinimumSize(100, 30);
    //        newButton->setMaximumSize(100, 30);

    //        listsLayout->addWidget(newButton);

    //        // connect onClick hook
    //        connect(newButton, &QPushButton::clicked,
    //                [this, i] { parseFolder(commonResrc->listinfo_[i].videoDirPath.c_str()); });
    //    }
    renderBtnList(listsBtnsLayout);

    // Set the container QWidget as the widget for the QScrollArea
    listsScrollArea->setWidget(listsContainer);

    // Set the video output of the media player to the video widget
    commonResrc->mediaPlayer_->setVideoOutput(videoWidget);

    // Connect signals and slots for window switch
    connect(ui->addListBtn, &QPushButton::clicked, this, &mainwindowm::switchToPage);

    // Connect signals and slots for media playback control
    connect(commonResrc->mediaPlayer_, &QMediaPlayer::positionChanged, this, &mainwindowm::updateProgressBar);
    connect(ui->progressbar, &QSlider::sliderMoved, this, &mainwindowm::onProgressbarSliderMoved);
    connect(ui->forward, &QPushButton::clicked, this, &mainwindowm::onForwardClicked);
    connect(ui->retreat, &QPushButton::clicked, this, &mainwindowm::onRetreatClicked);
    connect(ui->pause, &QPushButton::clicked, this, &mainwindowm::onPauseClicked);
    connect(ui->fullScreen, &QPushButton::clicked, this, &mainwindowm::toggleFullScreen);
    connect(ui->voicecontrolstrip, &QSlider::valueChanged, this, &mainwindowm::adjustVolume);
    connect(ui->voice, &QPushButton::clicked, this, &mainwindowm::toggleVoiceControlStrip);
}

// Destructor
mainwindowm::~mainwindowm() {
    delete commonResrc;
    delete ui;
}

void mainwindowm::renderBtnList(QHBoxLayout* btnLayout) {
    for (size_t i = 0; i < commonResrc->listinfo_.size(); i++) {
        QPushButton* newButton = new QPushButton();
        newButton->setText(commonResrc->listinfo_[i].name.c_str());
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);

        // 设置按钮的最小和最大大小
        newButton->setMinimumSize(100, 30);
        newButton->setMaximumSize(100, 30);

        btnLayout->addWidget(newButton);

        // connect onClick hook
        connect(newButton, &QPushButton::clicked,
                [this, i] { parseFolder(commonResrc->listinfo_[i].videoDirPath.c_str()); });
    }
}

// onPauseClicked() toggles the play/pause state of the media player.
// If the media player is currently playing, it pauses playback;
// otherwise, it starts or resumes playback.
void mainwindowm::onPauseClicked() {
    // Toggle play/pause state
    if (commonResrc->mediaPlayer_->state() == QMediaPlayer::PlayingState) {
        // If the media player is currently in the playing state, pause playback
        commonResrc->mediaPlayer_->pause();
    } else {
        // If the media player is not in the playing state, start or resume playback
        commonResrc->mediaPlayer_->play();
    }
}

// onForwardClicked() switches to the next video in the playlist.
void mainwindowm::onForwardClicked() {
    // Switch to the next video
    commonResrc->currentVideoIndex_ = (commonResrc->currentVideoIndex_ + 1) % commonResrc->videoPaths_.size();

    // Check if the current video index is within the valid range
    if (commonResrc->currentVideoIndex_ >= 0 && commonResrc->currentVideoIndex_ < commonResrc->videoPaths_.size()) {
        // Reset the playback position
        commonResrc->mediaPlayer_->setPosition(0);

        // Set up the media source for the new video
        QString videoPath = commonResrc->videoPaths_[commonResrc->currentVideoIndex_];
        commonResrc->mediaPlayer_->setMedia(QUrl::fromLocalFile(QFileInfo(videoPath).absoluteFilePath()));

        // Start playback
        commonResrc->mediaPlayer_->play();
    }
}

// onRetreatClicked() switches to the previous video in the playlist.
void mainwindowm::onRetreatClicked() {
    // Switch to the previous video
    commonResrc->currentVideoIndex_ =
        (commonResrc->currentVideoIndex_ - 1 + commonResrc->videoPaths_.size()) % commonResrc->videoPaths_.size();

    // Check if the current video index is within the valid range
    if (commonResrc->currentVideoIndex_ >= 0 && commonResrc->currentVideoIndex_ < commonResrc->videoPaths_.size()) {
        // Reset the playback position
        commonResrc->mediaPlayer_->setPosition(0);

        // Set up the media source for the new video
        QString videoPath = commonResrc->videoPaths_[commonResrc->currentVideoIndex_];
        commonResrc->mediaPlayer_->setMedia(QUrl::fromLocalFile(QFileInfo(videoPath).absoluteFilePath()));

        // Start playback
        commonResrc->mediaPlayer_->play();
    }
}

// onProgressbarSliderMoved() handles the sliderMoved signal of the progress bar.
// It sets the media player's position to the specified value, allowing the user
// to manually adjust the playback position by moving the progress bar slider.
// Params:
// - position: The new position set by the user through the progress bar slider.
void mainwindowm::onProgressbarSliderMoved(int position) {
    // Set the position moved by the slider to the media player
    commonResrc->mediaPlayer_->setPosition(position);
}

// updateProgressBar() updates the progress bar based on the current position of the media playback.
void mainwindowm::updateProgressBar(qint64 position) {
    // Update the current value of the progress bar
    qint64 totalDuration = commonResrc->mediaPlayer_->duration();

    if (totalDuration > 0) {
        // Set the range of the progress bar to the total duration of the media
        ui->progressbar->setRange(0, totalDuration);

        // Set the current value of the progress bar to the current position of the media playback
        ui->progressbar->setValue(position);
    }
}

// handleMediaStatusChanged() is triggered when the media status changes.
// If the media has loaded successfully (LoadedMedia), it starts playback and disconnects the signal
// to avoid unnecessary calls.
void mainwindowm::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        // Media has loaded successfully, start playback
        std::cout << "play video: " << commonResrc->currentVideoIndex_ << std::endl;
        //        disconnect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handleMediaStatusChanged);
        commonResrc->mediaPlayer_->play();
        isVideoPlaying = true;
        ui->video->show();
        if (ui->video->isVisible()) {
            resizeEvent(nullptr);
        }

    } else if (status == QMediaPlayer::EndOfMedia) {
        // Current video playback is complete, automatically play the next video

        // disconnect previous slots
        disconnect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                   &mainwindowm::handleMediaStatusChanged);

        commonResrc->currentVideoIndex_ = (commonResrc->currentVideoIndex_ + 1) % commonResrc->videoPaths_.size();
        std::cout << "end of video, switch to next: " << commonResrc->currentVideoIndex_ << std::endl;
        setMediaAndPlay();
    }
}

// setMediaAndPlay() sets up a signal-slot connection for media status changed,
// checks if the current video index is within the valid range, retrieves the path of the next video,
// and sets the media source to start playback.
void mainwindowm::setMediaAndPlay() {
    // Check if the current video index is within the valid range
    if (commonResrc->currentVideoIndex_ >= 0 && commonResrc->currentVideoIndex_ < commonResrc->videoPaths_.size()) {
        // Retrieve the path of the next video in the playlist
        QString videoPath = commonResrc->videoPaths_[commonResrc->currentVideoIndex_];

        // Set the media source to the current video
        commonResrc->mediaPlayer_->setMedia(QUrl::fromLocalFile(QFileInfo(videoPath).absoluteFilePath()));

        // Set up a signal-slot connection for media status changed
        connect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                &mainwindowm::handleMediaStatusChanged);
    }
}

// handleVideoSelection() is called when a video button is clicked. It receives the list of video paths
// and the current index, sets the current index, resets the playback position, and starts playback.
// Params:
// - commonResrc->videoPaths_: QStringList containing paths of all videos in the folder.
// - currentIndex: The index corresponding to the clicked button.
void mainwindowm::handleVideoSelection(const QStringList& videoPaths_, int currentIndex) {
    // Log the video paths and current index for debugging purposes
    qDebug() << "Button clicked. Video paths:" << videoPaths_;
    qDebug() << "Button clicked. currentIndex:" << currentIndex;

    // Set commonResrc->videoPaths_ and commonResrc->currentVideoIndex_
    this->commonResrc->videoPaths_ = commonResrc->videoPaths_;
    this->commonResrc->currentVideoIndex_ = currentIndex;

    // Reset the playback position
    commonResrc->mediaPlayer_->setPosition(0);

    // Start playback
    setMediaAndPlay();
}

// parseFolder() is called to parse the specified folder path for video files (".mp4").
// It creates thumbnail buttons for each video with corresponding preview images (.png or .jpg).
// The buttons are displayed in a vertical layout within a scroll area.
// Params:
// - folderPath: The path of the folder to parse for video files.
void mainwindowm::parseFolder(const QString& folderPath) {
    // Log the folder path for debugging purposes
    qDebug() << "Folder path:" << folderPath;

    // Create a QDir object for the specified folder path
    QDir dir(folderPath);

    // Retrieve a list of video files (.mp4) in the folder
    QStringList videoFiles = dir.entryList(QStringList() << "*.mp4", QDir::Files);

    // Get the QScrollArea object named thumbnailList
    QScrollArea* thumbnailScrollArea = ui->thumbnailList;

    // Create a new QWidget to serve as the parent control for the button's vertical layout
    QWidget* containerWidget = new QWidget(thumbnailScrollArea);

    // Create a QVBoxLayout for the buttons and add buttons to the layout
    QVBoxLayout* layout = new QVBoxLayout(containerWidget);

    // Iterate through each video file in the folder
    foreach (const QString& videoFile, videoFiles) {
        QString videoPath = dir.filePath(videoFile);
        commonResrc->videoPaths_.append(videoPath);

        QString baseName = QFileInfo(videoFile).baseName();
        QString imagePathPNG = dir.filePath(baseName + ".png");
        QString imagePathJPG = dir.filePath(baseName + ".jpg");

        // Check if preview images (.png or .jpg) exist for the current video
        if (QFileInfo::exists(imagePathPNG) || QFileInfo::exists(imagePathJPG)) {
            // Create a BtnConvert button with the video path
            BtnConvert* button = new BtnConvert(videoPath);

            // Set the button icon to the existing preview image (.png or .jpg)
            if (QFileInfo::exists(imagePathPNG)) {
                button->setIcon(QIcon(imagePathPNG));
            }
            if (QFileInfo::exists(imagePathJPG)) {
                button->setIcon(QIcon(imagePathJPG));
            }

            // Set the icon size and connect the button click signal to onButtonClicked slot
            button->setIconSize(QSize(250, 250));
            connect(button, &QPushButton::clicked, this, &mainwindowm::onButtonClicked);

            // Add the button to the layout
            layout->addWidget(button);
        }
    }

    // Set the container QWidget as the widget for the QScrollArea
    thumbnailScrollArea->setWidget(containerWidget);
}

// onButtonClicked() is triggered when a BtnConvert button is clicked.
// It retrieves the video path from the clicked button and calls handleVideoSelection()
// to handle the selection of the corresponding video in the playlist.
void mainwindowm::onButtonClicked() {
    // Attempt to cast the sender to BtnConvert
    BtnConvert* button = qobject_cast<BtnConvert*>(sender());
    if (button) {
        // Retrieve the video path from the clicked button
        QString videoPath = button->getVideoPath();

        // Find the index of the video path in the list and handle the video selection
        int index = commonResrc->videoPaths_.indexOf(videoPath);
        if (index != -1) {
            handleVideoSelection(commonResrc->videoPaths_, index);
        }
        ui->picturelist->hide();
    }
}

// switchToListset() is called to switch to the ListSet window.
// It hides the current MainWindow and shows a new ListSet window.
void mainwindowm::switchToListset() {
    // Close the current MainWindow
    hide();

    // Create a new ListSet window
    ListSet* listsetWindow = new ListSet();

    // Show the ListSet window
    listsetWindow->show();
}

// togglePictureList() toggles the visibility of the picturelist.
void mainwindowm::toggleFullScreen() {
    ui->picturelist->setHidden(!ui->picturelist->isHidden());
    updateGeometry();  // 强制更新布局
    repaint();         // 强制重绘

    // 手动触发 resizeEvent
    QResizeEvent* event = new QResizeEvent(size(), size());
    resizeEvent(event);
}

void mainwindowm::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    // 如果 picturelist 和 videoplayer 都没有被隐藏
    if (!ui->picturelist->isHidden() && !ui->video->isHidden()) {
        // 获取 QGroupBox 的初始宽度比例，可以根据需要进行调整
        float initialPicturelistWidthRatio = 0.3;

        // 获取 central 的宽度
        int centralWidth = this->width();

        // 计算 QGroupBox 的宽度
        int initialPicturelistWidth = static_cast<int>(centralWidth * initialPicturelistWidthRatio);

        // 设置 QGroupBox 的大小
        ui->picturelist->setFixedWidth(initialPicturelistWidth);
    }

    // 调用 updateGeometry 触发布局更新
    updateGeometry();
}

void mainwindowm::toggleVoiceControlStrip() {
    // 判断当前状态，如果是隐藏则显示，反之亦然
    if (ui->voicecontrolstrip->isHidden()) {
        ui->voicecontrolstrip->show();
    } else {
        ui->voicecontrolstrip->hide();
    }
}

void mainwindowm::adjustVolume(int volume) {
    // 将音量值映射到 QMediaPlayer 的音量范围（0 到 100）
    qreal volumeLevel = volume / 100.0;

    // 设置 QMediaPlayer 的音量
    commonResrc->mediaPlayer_->setVolume(static_cast<int>(volumeLevel * 100));
}

// RefreshList() refreshes the videolist and video path
void mainwindowm::RefreshList() {
    // Assuming ui->lists is now a QScrollArea
    QScrollArea* listsScrollArea = ui->lists;

    // Create a QWidget to serve as the container for the buttons
    QWidget* listsContainer = new QWidget(listsScrollArea);

    // Create a QHBoxLayout for the buttons
    if (listsBtnsLayout == nullptr) {
        listsBtnsLayout = new QHBoxLayout(listsContainer);
    }

    // Clear existing buttons
    QLayoutItem* child;
    while ((child = listsBtnsLayout->takeAt(0)) != nullptr) {
        std::cout << "delete in mainwindow small" << std::endl;
        delete child->widget();
        delete child;
    }

    delete commonResrc->fileUtil_;
    commonResrc->fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
    commonResrc->listinfo_ = commonResrc->fileUtil_->GetAllListsInfo();
    commonResrc->fileUtil_->PrintAll();
    for (size_t i = 0; i < commonResrc->listinfo_.size(); i++) {
        std::cout << commonResrc->listinfo_[i].name << std::endl;
    }
    renderBtnList(listsBtnsLayout);
}

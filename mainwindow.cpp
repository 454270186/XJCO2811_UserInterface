#include "mainwindow.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QProcess>
#include <QResizeEvent>
#include <QVBoxLayout>
#include "btnconvert.h"
#include "listset.h"
#include "ui_mainwindow.h"

#include <iostream>

// MainWindow constructor initializes the main window and its components.
MainWindow::MainWindow(QWidget* parent, MainWindowResource* cr)
    : QMainWindow(parent), ui(new Ui::MainWindow), videoWidget(new QVideoWidget(this)) {
    // Init common resource
    commonResrc = cr;

    // Set up the user interface
    ui->setupUi(this);
    QFile file2("../XJCO2811_UserInterface/mainwindow.qss");
    QString StyleSheet;
    if (file2.open(QFile::ReadOnly)) {
        StyleSheet += QLatin1String(file2.readAll());
        file2.close();
    } else {
        qDebug() << "File does not exist: " << file2.fileName();
    }

    if (!StyleSheet.isEmpty()) {
        this->setStyleSheet(StyleSheet);
    } else {
        qDebug() << "Current directory:" << QDir::currentPath();
    }
    ui->lists->setStyleSheet("QScrollArea { border: 0; }");

    // Assuming you want to set the initial size to 1000x700
    setGeometry(100, 100, 1000, 700);

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
    //    for (size_t i = 0; i < listInfos_.size(); i++) {
    //        QPushButton* newButton = new QPushButton();
    //        newButton->setText(listInfos_[i].name.c_str());
    //        newButton->setCheckable(true);
    //        newButton->setAutoExclusive(true);

    //        listsLayout->addWidget(newButton);

    //        // connect onClick hook
    //        connect(newButton, &QPushButton::clicked, [this, i] { parseFolder(listInfos_[i].videoDirPath.c_str()); });
    //    }
    renderBtnList(listsBtnsLayout);

    // Set the container QWidget as the widget for the QScrollArea
    listsScrollArea->setWidget(listsContainer);

    // Set the video output of the media player to the video widget
    commonResrc->mediaPlayer_->setVideoOutput(videoWidget);

    // Connect signals and slots for window switch
    connect(ui->addListBtn, &QPushButton::clicked, this, &MainWindow::switchToPage);

    // Connect signals and slots for media playback control
    connect(commonResrc->mediaPlayer_, &QMediaPlayer::positionChanged, this, &MainWindow::updateProgressBar);
    connect(ui->progressbar, &QSlider::sliderMoved, this, &MainWindow::onProgressbarSliderMoved);
    connect(ui->forward, &QPushButton::clicked, this, &MainWindow::onForwardClicked);
    connect(ui->retreat, &QPushButton::clicked, this, &MainWindow::onRetreatClicked);
    connect(ui->pause, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(ui->fullScreen, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
    connect(ui->voicecontrolstrip, &QSlider::valueChanged, this, &MainWindow::adjustVolume);
}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::renderBtnList(QHBoxLayout* btnLayout) {
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
void MainWindow::onPauseClicked() {
    // Toggle play/pause state
    if (commonResrc->mediaPlayer_->state() == QMediaPlayer::PlayingState) {
        // If the media player is currently in the playing state, pause playback
        commonResrc->mediaPlayer_->pause();

        // Set the pause icon
        ui->pause->setIcon(QIcon("../XJCO2811_UserInterface/icons/play-circle.svg"));
    } else {
        // If the media player is not in the playing state, start or resume playback
        commonResrc->mediaPlayer_->play();

        // Set the play icon
        ui->pause->setIcon(QIcon("../XJCO2811_UserInterface/icons/pause.svg"));
    }
}

// onForwardClicked() switches to the next video in the playlist.
void MainWindow::onForwardClicked() {
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
void MainWindow::onRetreatClicked() {
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
void MainWindow::onProgressbarSliderMoved(int position) {
    // Set the position moved by the slider to the media player
    commonResrc->mediaPlayer_->setPosition(position);
}

// updateProgressBar() updates the progress bar based on the current position of the media playback.
void MainWindow::updateProgressBar(qint64 position) {
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
void MainWindow::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
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
                   &MainWindow::handleMediaStatusChanged);

        commonResrc->currentVideoIndex_ = (commonResrc->currentVideoIndex_ + 1) % commonResrc->videoPaths_.size();
        std::cout << "end of video, switch to next: " << commonResrc->currentVideoIndex_ << std::endl;
        setMediaAndPlay();
    }
}

// setMediaAndPlay() sets up a signal-slot connection for media status changed,
// checks if the current video index is within the valid range, retrieves the path of the next video,
// and sets the media source to start playback.
void MainWindow::setMediaAndPlay() {
    // Check if the current video index is within the valid range
    if (commonResrc->currentVideoIndex_ >= 0 && commonResrc->currentVideoIndex_ < commonResrc->videoPaths_.size()) {
        // Retrieve the path of the next video in the playlist
        QString videoPath = commonResrc->videoPaths_[commonResrc->currentVideoIndex_];

        // Set the media source to the current video
        commonResrc->mediaPlayer_->setMedia(QUrl::fromLocalFile(QFileInfo(videoPath).absoluteFilePath()));

        // Set up a signal-slot connection for media status changed
        connect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                &MainWindow::handleMediaStatusChanged);
    }
}

// handleVideoSelection() is called when a video button is clicked. It receives the list of video paths
// and the current index, sets the current index, resets the playback position, and starts playback.
// Params:
// - commonResrc->videoPaths_: QStringList containing paths of all videos in the folder.
// - currentIndex: The index corresponding to the clicked button.
void MainWindow::handleVideoSelection(const QStringList& videoPaths_, int currentIndex) {
    // Log the video paths and current index for debugging purposes
    qDebug() << "Button clicked. Video paths:" << videoPaths_;
    qDebug() << "Button clicked. currentIndex:" << currentIndex;

    // Set commonResrc->videoPaths_ and commonResrc->currentVideoIndex_
    this->commonResrc->videoPaths_ = videoPaths_;
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
void MainWindow::parseFolder(const QString& folderPath) {
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
    // Clear video paths
    commonResrc->videoPaths_.clear();
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
            connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

            // Add the button to the layout
            layout->addWidget(button);
        } else {
            std::cout << "video cover not found, make it..." << std::endl;
            QProcess ffmpeg;
            ffmpeg.setProgram("../XJCO2811_UserInterface/bin/ffmpeg");
            ffmpeg.setArguments({"-i", videoPath, "-vframes", "1", imagePathPNG});
            ffmpeg.start();
            ffmpeg.waitForFinished();

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
            connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

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
void MainWindow::onButtonClicked() {
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
void MainWindow::switchToListset() {
    // Close the current MainWindow
    hide();

    // Create a new ListSet window
    ListSet* listsetWindow = new ListSet();

    // Show the ListSet window
    listsetWindow->show();
}

// togglePictureList() toggles the visibility of the picturelist.
void MainWindow::toggleFullScreen() {
    ui->picturelist->setHidden(!ui->picturelist->isHidden());
    updateGeometry();  // 强制更新布局
    repaint();         // 强制重绘

    // 手动触发 resizeEvent
    QResizeEvent* event = new QResizeEvent(size(), size());
    resizeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    // 检测是否有一个被隐藏
    bool isPictureListHidden = ui->picturelist->isHidden();
    bool isVideoHidden = ui->video->isHidden();

    // 如果 picturelist 和 videoplayer 其中有一个被隐藏，自适应宽度
    if (isPictureListHidden != isVideoHidden) {
        if (isPictureListHidden) {
            // 如果 picturelist 被隐藏，自适应 video 的宽度
            int videoWidth = width();
            ui->video->setFixedWidth(videoWidth);
        } else {
            // 如果 video 被隐藏，自适应 picturelist 的宽度
            int picturelistWidth = width();
            ui->picturelist->setFixedWidth(picturelistWidth);
        }
    } else {
        // 如果 picturelist 和 videoplayer 都没有被隐藏，立即应用大小调整逻辑
        int picturelistWidth = width() * 0.3;
        int videoWidth = width() * 0.7;

        // 为 picturelist 设置最小宽度（根据需要调整此值）
        int minPicturelistWidth = 200;

        // 确保 picturelist 有一个最小宽度
        if (picturelistWidth < minPicturelistWidth) {
            picturelistWidth = minPicturelistWidth;
            videoWidth = width() - picturelistWidth;
        }

        // 设置 QGroupBox 的大小
        ui->picturelist->setFixedWidth(picturelistWidth);
        ui->video->setFixedWidth(videoWidth);
    }

    // 调用 updateGeometry 触发布局更新
    updateGeometry();

    // 强制重绘
    repaint();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->videoBox) {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入 videoBox 区域，显示音量条
            ui->voicecontrolstrip->show();
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开 videoBox 区域，隐藏音量条
            ui->voicecontrolstrip->hide();
        }
    }

    // 将事件传递给基类处理
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::adjustVolume(int volume) {
    // 将音量值映射到 QMediaPlayer 的音量范围（0 到 100）
    qreal volumeLevel = volume / 100.0;

    // 设置 QMediaPlayer 的音量
    commonResrc->mediaPlayer_->setVolume(static_cast<int>(volumeLevel * 100));
}

// RefreshList() refreshes the videolist and video path
void MainWindow::RefreshList() {
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
        std::cout << "delete in mainwindow big" << std::endl;
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

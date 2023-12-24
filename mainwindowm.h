#ifndef MAINWINDOWM_H
#define MAINWINDOWM_H

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QSlider>
#include <QStringList>
#include <QVideoWidget>
#include <QWidget>

#include <iostream>
#include <vector>

#include "mainwindowresource.h"

namespace Ui {
class mainwindowm;
}

class mainwindowm : public QMainWindow {
    Q_OBJECT

public:
    Ui::mainwindowm* ui;
    explicit mainwindowm(QWidget* parent = nullptr, MainWindowResource* cr = nullptr);
    ~mainwindowm();

    // Play() and Pause() are exposed to PageManager
    // to control the video play or pause
    void Play() { commonResrc->mediaPlayer_->play(); }
    void Pause() { commonResrc->mediaPlayer_->pause(); }

    QVideoWidget* getVideoOutput() { return this->videoWidget; }

    void RefreshList();
    void RenderTheme();

    void DisconnectMediaplayerEvent() {
        disconnect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                   &mainwindowm::handleMediaStatusChanged);
    }
    void ConnectMediaplayerEvent() {
        connect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                &mainwindowm::handleMediaStatusChanged);
    }

signals:
    void switchPage(int pageIndex);

public slots:

    // Slot to switch between windows
    void switchToListset();

private slots:
    // Slot for the pause button click event
    void onPauseClicked();

    // Slot for the forward button click event
    void onForwardClicked();

    // Slot for the retreat button click event
    void onRetreatClicked();

    // Slot for the progress bar slider move event
    void onProgressbarSliderMoved(int position);

    // Slot to update the progress bar based on media position
    void updateProgressBar(qint64 position);

    void onButtonClicked();
    void parseFolder(const QString& folderPath);
    void handleVideoSelection(const QStringList& videoPaths, int currentIndex);  // 接口函数
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void toggleFullScreen();
    void switchToPage() { emit switchPage(3); }
    void adjustVolume(int volume);
    void toggleVoiceControlStrip();

private:
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList& videoPaths, int currentIndex);
    void resizeEvent(QResizeEvent* event) override;
    void renderBtnList(QHBoxLayout* btnLayout);
    void keyPressEvent(QKeyEvent* event) override;

    QVideoWidget* videoWidget;
    QHBoxLayout* listsBtnsLayout;
    bool isVideoPlaying;

    MainWindowResource* commonResrc;
};

#endif  // MAINWINDOWM_H

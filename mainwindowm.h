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
#include <QTimer>
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
    void switchToListset();

private slots:
    void onPauseClicked();
    void onForwardClicked();
    void onRetreatClicked();
    void onProgressbarSliderMoved(int position);
    void updateProgressBar(qint64 position);
    void onButtonClicked();
    void parseFolder(const QString& folderPath);
    void handleVideoSelection(const QStringList& videoPaths, int currentIndex);  // 接口函数
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void toggleFullScreen();
    void switchToPage() { emit switchPage(3); }
    void adjustVolume(int volume);
    void toggleVoiceControlStrip();
    void onScreenShotClicked();

private:
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList& videoPaths, int currentIndex);
    void resizeEvent(QResizeEvent* event) override;
    void renderBtnList(QHBoxLayout* btnLayout);
    void keyPressEvent(QKeyEvent* event) override;
    void increaseVolume();
    void decreaseVolume();
    void hideVolumeControl();
    void loadStyleSheet(const QString& sheetName);

    bool isVideoPlaying;
    QVideoWidget* videoWidget;
    QHBoxLayout* listsBtnsLayout;
    QTimer* volumeControlTimer;
    MainWindowResource* commonResrc;
};

#endif  // MAINWINDOWM_H

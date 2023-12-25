#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    Ui::MainWindow* ui;
    explicit MainWindow(QWidget* parent = nullptr, MainWindowResource* cr = nullptr);
    ~MainWindow();

    // Play() and Pause() are exposed to PageManager
    // to control the video play or pause
    void Play() { commonResrc->mediaPlayer_->play(); }
    void Pause() { commonResrc->mediaPlayer_->pause(); }

    QVideoWidget* getVideoOutput() { return this->videoWidget; }

    void RefreshList();
    void RenderTheme();

    void DisconnectMediaplayerEvent() {
        disconnect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                   &MainWindow::handleMediaStatusChanged);
    }
    void ConnectMediaplayerEvent() {
        connect(commonResrc->mediaPlayer_, &QMediaPlayer::mediaStatusChanged, this,
                &MainWindow::handleMediaStatusChanged);
    }

signals:
    void switchPage(int pageIndex);
    void snapshot();

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
    void handleVideoSelection(const QStringList& videoPaths, int currentIndex);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void toggleFullScreen();
    void switchToPage() { emit switchPage(1); }
    void adjustVolume(int volume);
    void onScreenShotClicked();

private:
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList& videoPaths, int currentIndex);
    void renderBtnList(QHBoxLayout* btnLayout);
    void resizeEvent(QResizeEvent* event) override;
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
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

#endif  // MAINWINDOW_H

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

    // Exposed to PageManager, used for page state synchronization
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

private slots:
    // Slots for video control events
    void onPauseClicked();
    void onForwardClicked();
    void onRetreatClicked();
    void onProgressbarSliderMoved(int position);
    void updateProgressBar(qint64 position);

    // Slots for UI events
    void onButtonClicked();
    void parseFolder(const QString& folderPath);
    void handleVideoSelection(const QStringList& videoPaths, int currentIndex);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void toggleFullScreen();
    void adjustVolume(int volume);
    void onScreenShotClicked();

    // Slots for page switch
    void switchToPage() { emit switchPage(1); }

private:
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList& videoPaths, int currentIndex);
    void renderBtnList(QHBoxLayout* btnLayout);
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void increaseVolume();
    void decreaseVolume();
    void hideVolumeControl();
    void loadStyleSheet(const QString& sheetName);
    virtual bool eventFilter(QObject* obj, QEvent* event) override;

    QVideoWidget* videoWidget;
    QHBoxLayout* listsBtnsLayout;
    QTimer* volumeControlTimer;
    MainWindowResource* commonResrc;
};

#endif  // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QPushButton>
#include <QStringList>
#include <QVideoWidget>
#include <vector>

#include "fileutil.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    Ui::MainWindow* ui;
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // Play() and Pause() are exposed to PageManager
    // to control the video play or pause
    void Play() { mediaPlayer->play(); }
    void Pause() { mediaPlayer->pause(); }

    void RefreshList();

signals:
    void switchPage(int pageIndex);

public slots:
    // Slot to handle folder path changes
    void setFolderPath(const QString& path);

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
    void switchToPage() { emit switchPage(1); }

private:
    bool isVideoPlaying;
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList& videoPaths, int currentIndex);
    void renderBtnList(QHBoxLayout* btnLayout);
    void resizeEvent(QResizeEvent* event) override;
    bool isFullScreen = false;
    QRect normalGeometry;

    QMediaPlayer* mediaPlayer;
    QVideoWidget* videoWidget;
    QStringList videoPaths;  // 存储所有视频路径
    QHBoxLayout* listsBtnsLayout;
    FileUtil* fileUtil_;
    std::vector<ListInfo> listInfos_;
    int currentVideoIndex;  // 当前播放的视频索引
};

#endif  // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // Slot to handle folder path changes
    void setFolderPath(const QString &path);

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
    void parseFolder(const QString &folderPath);
    void handleVideoSelection(const QStringList &videoPaths, int currentIndex); // 接口函数

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QStringList videoPaths; // 存储所有视频路径
    int currentVideoIndex;// 当前播放的视频索引
    void setMediaAndPlay();
    void startPlaylistFromParameters(const QStringList &videoPaths, int currentIndex);

};

#endif // MAINWINDOW_H

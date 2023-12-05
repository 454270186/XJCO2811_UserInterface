// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>

    namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setFolderPath(const QString &path);

private slots:
    void on_pause_clicked();
    void on_forward_clicked();
    void on_retreat_clicked();
    void on_progressbar_sliderMoved(int position);
    void updateProgressBar(qint64 position);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
};

#endif // MAINWINDOW_H

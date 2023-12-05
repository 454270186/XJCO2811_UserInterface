// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>

<<<<<<< HEAD
    namespace Ui {
    class MainWindow;
=======
namespace Ui {
class MainWindow;
namespace Ui {
class MainWindow;
>>>>>>> main
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
<<<<<<< HEAD
    void setFolderPath(const QString &path);

private slots:
    void on_pause_clicked();
    void on_forward_clicked();
    void on_retreat_clicked();
    void on_progressbar_sliderMoved(int position);
=======
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
>>>>>>> main
    void updateProgressBar(qint64 position);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
};

#endif // MAINWINDOW_H

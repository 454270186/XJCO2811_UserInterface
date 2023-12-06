#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked();
    void parseFolder(const QString &folderPath);
    void handleVideoSelection(const QStringList &videoPaths, int currentIndex); // 接口函数

private:
    Ui::MainWindow *ui;
    QStringList videoPaths; // 存储所有视频路径
};

#endif // MAINWINDOW_H

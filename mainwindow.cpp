#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "btnconvert.h"
#include <QDir>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    parseFolder("/Users/haoyue/Desktop/XJCO2811_UserInterface-hy/XJCO2811_UserInterface/videos"); // 使用前要替换为你本地的文件夹路径
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::parseFolder(const QString &folderPath) {
    QDir dir(folderPath);
    QStringList videoFiles = dir.entryList(QStringList() << "*.mp4", QDir::Files);

    foreach (const QString &videoFile, videoFiles) {
        QString videoPath = dir.filePath(videoFile);
        videoPaths.append(videoPath);

        QString baseName = QFileInfo(videoFile).baseName();
        QString imagePath = dir.filePath(baseName + ".png");

        if (QFileInfo::exists(imagePath)) {
            BtnConvert *button = new BtnConvert(videoPath);
            button->setIcon(QIcon(imagePath));
            button->setIconSize(QSize(100, 100));
            connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

            QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->picture->layout());
            if (layout) {
                layout->addWidget(button);
            }
        }
    }
}

void MainWindow::onButtonClicked() {
    BtnConvert *button = qobject_cast<BtnConvert *>(sender());
    if (button) {
        QString videoPath = button->getVideoPath();
        int index = videoPaths.indexOf(videoPath);
        if (index != -1) {
            handleVideoSelection(videoPaths, index);
        }
    }
}

void MainWindow::handleVideoSelection(const QStringList &videoPaths, int currentIndex) {
    // 将视频路径数组和索引传递给接口
    // 这里实现接口调用的逻辑
    qDebug() << "Button clicked. Video paths:" << videoPaths; // VideoPaths是一个字符串数组，存储文件夹里所有的视频路径
    qDebug() << "Button clicked. currentIndex:" << currentIndex;// currentIndex是一个整型，显示当前按钮对应的索引
}

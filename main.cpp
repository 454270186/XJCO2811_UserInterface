#include <QApplication>
#include "pagemanager.h"

#include <QProcess>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    //    PageManager pagemanager;
    //    pagemanager.show();

    QString ffmpegCommand =
        "../XJCO2811_UserInterface/bin/ffmpeg -i ../XJCO2811_UserInterface/videos/demo/wang.mp4 -vframes 1 "
        "../XJCO2811_UserInterface/output.jpg";
    QProcess ffmpegProcess;
    ffmpegProcess.start(ffmpegCommand);
    ffmpegProcess.waitForFinished();

    app.exec();
}

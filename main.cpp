#include <QApplication>
#include "pagemanager.h"

#include <QProcess>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    //    PageManager pagemanager;
    //    pagemanager.show();
    QProcess ffmpegProcess;
    ffmpegProcess.setProgram("../XJCO2811_UserInterface/bin/ffmpeg");
    ffmpegProcess.setArguments({"-i", "../XJCO2811_UserInterface/videos/demo/wang.mp4", "-vframes", "1",
                                "../XJCO2811_UserInterface/output.jpg"});

    ffmpegProcess.start();
    ffmpegProcess.waitForFinished();

    app.exec();
}

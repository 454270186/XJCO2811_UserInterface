#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // 如果命令行参数数量大于1，表示有额外的参数传递
    if (a.arguments().count() > 1) {
        // 将第二个参数作为文件夹路径传递给主窗口
        w.setFolderPath(a.arguments().at(1));
    }

    w.show();
    return a.exec();
}


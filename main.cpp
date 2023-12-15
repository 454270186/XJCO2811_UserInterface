#include <QApplication>
#include <QFile>
#include "pagemanager.h"
#include <QString>
#include <QDir>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QFile file("../XJCO2811_UserInterface/mainwindow.qss");
    if (file.open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(StyleSheet);
        file.close();
    } else {
    qDebug() << "File does not exist: " << file.fileName();
    qDebug() << "Current directory:" << QDir::currentPath();

    }

    PageManager pagemanager;
    pagemanager.show();

    app.exec();
}

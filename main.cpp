#include <QApplication>

#include "mainwindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow w;
    w.show();
    int i; int j; int c;

    app.exec();
}
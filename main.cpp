#include <QApplication>

#include "mainwindow.h"
#include "listset.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    ListSet s;
    s.show();

    app.exec();
}

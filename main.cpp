#include <QApplication>

#include "listset.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    ListSet w;
    w.show();

    app.exec();
}

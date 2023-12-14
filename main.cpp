#include <QApplication>

#include "pagemanager.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    PageManager pagemanager;
    pagemanager.show();

    app.exec();
}

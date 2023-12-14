#include <QApplication>

#include "pagemanager.h"
// #include "listset.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    PageManager pagemanager;
    pagemanager.show();

    // ListSet* listSet = new ListSet();
    // listSet->show();

    app.exec();
}

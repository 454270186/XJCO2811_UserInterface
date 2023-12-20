#include <QApplication>
#include "pagemanager.h"
#include "listset.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // PageManager pagemanager;
    // pagemanager.show();

    ListSet* listset = new ListSet();
    listset->show();

    app.exec();
}

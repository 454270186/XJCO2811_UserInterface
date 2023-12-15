#include <QApplication>

#include "pagemanager.h"
#include "listsetsmall.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // PageManager pagemanager;
    // pagemanager.show();
    ListSetSmall* listsetsmall = new ListSetSmall();
    listsetsmall->show();

    app.exec();
}

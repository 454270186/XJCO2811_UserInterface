#include "share.h"
#include "ui_share.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    share testShare;
    testShare.show();

    return a.exec();
}

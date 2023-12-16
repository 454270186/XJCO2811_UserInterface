#include "pagemanager.h"

PageManager::PageManager(QWidget* parent) : QMainWindow{parent} {
    stackPage = new QStackedWidget(this);

    mainwindow = new MainWindow(this);
    mainwindowSmall = new mainwindowm(this);
    listset = new ListSet(this);
    listsetSmall = new ListSetSmall(this);

    stackPage->addWidget(mainwindow);
    stackPage->addWidget(listset);
    stackPage->addWidget(mainwindowSmall);
    stackPage->addWidget(listsetSmall);

    connect(mainwindow, &MainWindow::switchPage, this, &PageManager::switchToPage);
    connect(listset, &ListSet::switchPage, this, &PageManager::switchToPage);
    connect(this, &PageManager::resized, this, &PageManager::changeWindows);

    setCentralWidget(stackPage);
}

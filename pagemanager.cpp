#include "pagemanager.h"
#include "mainwindowresource.h"

PageManager::PageManager(QWidget* parent) : QMainWindow{parent} {
    stackPage = new QStackedWidget(this);

    // distribute common resource
    commonResrc = new MainWindowResource();
    commonResrc->mediaPlayer_ = new QMediaPlayer(this);
    mainwindowSmall = new mainwindowm(this, commonResrc);
    mainwindow = new MainWindow(this, commonResrc);

    // distribute listset common resource
    listsetResrc = new ListSetResource();
    listset = new ListSet(this, listsetResrc);
    listsetSmall = new ListSetSmall(this, listsetResrc);

    stackPage->addWidget(mainwindow);
    stackPage->addWidget(listset);
    stackPage->addWidget(mainwindowSmall);
    stackPage->addWidget(listsetSmall);

    connect(mainwindow, &MainWindow::switchPage, this, &PageManager::switchToPage);
    connect(listset, &ListSet::switchPage, this, &PageManager::switchToPage);
    connect(this, &PageManager::resized, this, &PageManager::changeWindows);

    setCentralWidget(stackPage);
}

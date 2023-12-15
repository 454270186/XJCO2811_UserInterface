#include "pagemanager.h"

PageManager::PageManager(QWidget* parent) : QMainWindow{parent} {
    stackPage = new QStackedWidget(this);

    mainwindow = new MainWindow(this);
    listset = new ListSet(this);

    stackPage->addWidget(mainwindow);
    stackPage->addWidget(listset);

    connect(mainwindow, &MainWindow::switchPage, this, &PageManager::switchToPage);
    connect(listset, &ListSet::switchPage, this, &PageManager::switchToPage);
    connect(this, &PageManager::resized, this, &PageManager::changeWindows);

    setCentralWidget(stackPage);
}

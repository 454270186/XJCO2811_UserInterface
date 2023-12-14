#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QMainWindow>
#include <QStackedWidget>
#include "listset.h"
#include "mainwindow.h"

class PageManager : public QMainWindow {
    Q_OBJECT
public:
    explicit PageManager(QWidget* parent = nullptr);

private slots:
    void switchToPage(int pageIndex) {
        if (pageIndex == 1) {
            mainwindow->Pause();
        } else {
            mainwindow->Play();
        }
        stackPage->setCurrentIndex(pageIndex);
    }

private:
    QStackedWidget* stackPage;
    MainWindow* mainwindow;
    ListSet* listset;
};

#endif  // PAGEMANAGER_H

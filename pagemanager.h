#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>
#include "faq.h"
#include "listset.h"
#include "listsetsmall.h"
#include "mainwindow.h"
#include "mainwindowm.h"

#include <iostream>

enum PageIndex { MAINWINDOW = 0, LISTSET = 1, MAINWINDOW_SMALL = 2, LISTSET_SMALL = 3, FAQ = 4 };

class PageManager : public QMainWindow {
    Q_OBJECT
public:
    explicit PageManager(QWidget* parent = nullptr);
    ~PageManager();
signals:
    void resized(const QSize& size);

private slots:
    void switchToPage(int pageIndex);
    void changeWindows(const QSize& size);

protected:
    void resizeEvent(QResizeEvent* event) override { emit resized(event->size()); }

private:
    QStackedWidget* stackPage;

    MainWindowResource* commonResrc;
    ListSetResource* listsetResrc;

    MainWindow* mainwindow;        // page index 0
    ListSet* listset;              // page index 1
    mainwindowm* mainwindowSmall;  // page index 2
    ListSetSmall* listsetSmall;    // page index 3
    Faq* faq;                      // page index 4
};

#endif  // PAGEMANAGER_H

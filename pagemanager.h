#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>
#include "listset.h"
#include "listsetsmall.h"
#include "mainwindow.h"
#include "mainwindowm.h"

class PageManager : public QMainWindow {
    Q_OBJECT
public:
    explicit PageManager(QWidget* parent = nullptr);

signals:
    void resized(const QSize& size);

private slots:
    void switchToPage(int pageIndex) {
        if (pageIndex == 1) {
            mainwindow->Pause();
        } else {
            // refresh video list before page switch
            mainwindow->RefreshList();

            mainwindow->Play();
        }
        stackPage->setCurrentIndex(pageIndex);
    }

    void changeWindows(const QSize& size) {
        QSize thresholdSize(600, 200000);

        if (size.width() >= thresholdSize.width()) {
            if (stackPage->currentIndex() == 2 || stackPage->currentIndex() == 3) {
                if (stackPage->currentIndex() == 2) {
                    stackPage->setCurrentIndex(0);
                } else if (stackPage->currentIndex() == 3) {
                    listset->RefreshList();
                    stackPage->setCurrentIndex(1);
                }
            }
        } else {
            if (stackPage->currentIndex() == 0 || stackPage->currentIndex() == 1) {
                // from big to small
                if (stackPage->currentIndex() == 0) {
                    stackPage->setCurrentIndex(2);
                } else if (stackPage->currentIndex() == 1) {
                    listsetSmall->RefreshList();
                    stackPage->setCurrentIndex(3);
                }
            }
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override { emit resized(event->size()); }

private:
    QStackedWidget* stackPage;

    MainWindow* mainwindow;        // page index 0
    ListSet* listset;              // page index 1
    mainwindowm* mainwindowSmall;  // page index 2
    ListSetSmall* listsetSmall;    // page index 3
};

#endif  // PAGEMANAGER_H

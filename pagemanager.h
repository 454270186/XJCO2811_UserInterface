#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>
#include "listset.h"
#include "listsetsmall.h"
#include "mainwindow.h"
#include "mainwindowm.h"
#include "faq.h"

#include <iostream>

class PageManager : public QMainWindow {
    Q_OBJECT
public:
    explicit PageManager(QWidget* parent = nullptr);
signals:
    void resized(const QSize& size);

private slots:
    void switchToPage(int pageIndex) {
        if (pageIndex == 1 || pageIndex == 3) {
            if (pageIndex == 1) {
                mainwindow->Pause();
            } else {
                mainwindowSmall->Pause();
            }
        } else if (pageIndex == 0 || pageIndex == 2) {
            // refresh video list before page switch
            if (pageIndex == 0) {
                mainwindow->RefreshList();
                mainwindow->Play();
            } else {
                mainwindowSmall->RefreshList();
                mainwindowSmall->Play();
            }
        }
        std::cout << "page index: " << pageIndex << std::endl;
        stackPage->setCurrentIndex(pageIndex);
    }

    void changeWindows(const QSize& size) {
        QSize thresholdSize(600, 200000);

        if (size.width() >= thresholdSize.width()) {
            if (stackPage->currentIndex() == 2 || stackPage->currentIndex() == 3) {
                // from small to big
                if (stackPage->currentIndex() == 2) {
                    commonResrc->mediaPlayer_->setVideoOutput(mainwindow->getVideoOutput());
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
                    commonResrc->mediaPlayer_->setVideoOutput(mainwindowSmall->getVideoOutput());
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

    MainWindowResource* commonResrc;
    ListSetResource* listsetResrc;

    MainWindow* mainwindow;        // page index 0
    ListSet* listset;              // page index 1
    mainwindowm* mainwindowSmall;  // page index 2
    ListSetSmall* listsetSmall;    // page index 3
    Faq* faq;                      // page index 4
};

#endif  // PAGEMANAGER_H

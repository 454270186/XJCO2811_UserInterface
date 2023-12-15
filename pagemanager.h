#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>
#include "listset.h"
#include "mainwindow.h"

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
            mainwindow->Play();
        }
        stackPage->setCurrentIndex(pageIndex);
    }

    void changeWindows(const QSize& size) {
        QSize thresholdSize(600, 200000);

        if (size.width() >= thresholdSize.width()) {
            stackPage->setCurrentIndex(0);
        } else {
            stackPage->setCurrentIndex(1);
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override { emit resized(event->size()); }

private:
    QStackedWidget* stackPage;
    MainWindow* mainwindow;
    ListSet* listset;
};

#endif  // PAGEMANAGER_H

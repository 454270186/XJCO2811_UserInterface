#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QPixmap>
#include <QProcess>
#include <QResizeEvent>
#include <QScreen>
#include <QUuid>
#include <QVBoxLayout>

#include "mainwindowresource.h"
#include "pagemanager.h"

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
    faq = new Faq(this, listsetResrc);

    stackPage->addWidget(mainwindow);
    stackPage->addWidget(listset);
    stackPage->addWidget(mainwindowSmall);
    stackPage->addWidget(listsetSmall);
    stackPage->addWidget(faq);

    connect(mainwindow, &MainWindow::switchPage, this, &PageManager::switchToPage);
    connect(mainwindowSmall, &mainwindowm::switchPage, this, &PageManager::switchToPage);
    connect(listset, &ListSet::switchPage, this, &PageManager::switchToPage);
    connect(listsetSmall, &ListSetSmall::switchPage, this, &PageManager::switchToPage);
    connect(faq, &Faq::switchPage, this, &PageManager::switchToPage);
    connect(faq, &Faq::stopReading, this, &PageManager::handleFaqStopReading);
    connect(this, &PageManager::resized, this, &PageManager::changeWindows);

    // snapshot
    connect(mainwindow, &MainWindow::snapshot, this, &PageManager::handleScreenShot);
    connect(mainwindowSmall, &mainwindowm::snapshot, this, &PageManager::handleScreenShot);

    setCentralWidget(stackPage);
}

PageManager::~PageManager() {
    // stop video play
    commonResrc->mediaPlayer_->pause();

    // release resoucre
    delete commonResrc;
    delete listsetResrc;
    delete faq;
    delete mainwindow;
    delete mainwindowSmall;
    delete listset;
    delete listsetSmall;
}

void PageManager::handleFaqStopReading() {
    faq->stopReadingAndReset();
}

void PageManager::handleScreenShot() {
    QScreen* screen = QGuiApplication::primaryScreen();

    QRect videoRect = this->geometry();

    QPixmap screenshot = screen->grabWindow(0, videoRect.x(), videoRect.y(), videoRect.width(), videoRect.height());

    QString directory = "../XJCO2811_UserInterface/snapshot";
    QDir dir(directory);
    if (!dir.exists()) {
        if (!dir.mkpath(directory)) {
            std::cerr << "Error: Failed to create directory: " << directory.toStdString() << std::endl;
            return;
        }
    }

    QUuid uuid = QUuid::createUuid();
    QString snapPath = "../XJCO2811_UserInterface/snapshot/" + uuid.toString() + ".jpg";
    if (!snapPath.isEmpty()) {
        std::cout << "cover!!!" << std::endl;
        screenshot.save(snapPath);
    }
}

void PageManager::switchToPage(int pageIndex) {
    if (pageIndex == PageIndex::LISTSET || pageIndex == PageIndex::LISTSET_SMALL) {
        if (pageIndex == PageIndex::LISTSET) {
            listset->RenderTheme();
            mainwindow->Pause();
        } else {
            listsetSmall->RenderTheme();
            mainwindowSmall->Pause();
        }
    } else if (pageIndex == PageIndex::MAINWINDOW || pageIndex == PageIndex::MAINWINDOW_SMALL) {
        // refresh video list before page switch
        // sync theme
        commonResrc->isChineseLanguage_ = listsetResrc->isChineseLanguage_;
        if (pageIndex == PageIndex::MAINWINDOW) {
            commonResrc->mediaPlayer_->setVideoOutput(mainwindow->getVideoOutput());
            mainwindow->RenderTheme();
            mainwindow->RefreshList();
            mainwindow->Play();
        } else {
            commonResrc->mediaPlayer_->setVideoOutput(mainwindowSmall->getVideoOutput());
            mainwindowSmall->RenderTheme();
            mainwindowSmall->RefreshList();
            mainwindowSmall->Play();
        }
    } else if (pageIndex == PageIndex::FAQ) {
        faq->RenderTheme();
    }

    std::cout << "page index: " << pageIndex << std::endl;
    stackPage->setCurrentIndex(pageIndex);
}

void PageManager::changeWindows(const QSize& size) {
    QSize thresholdSize(550, 200000);

    if (size.width() >= thresholdSize.width()) {
        // from small to big
        if (stackPage->currentIndex() == PageIndex::MAINWINDOW_SMALL ||
            stackPage->currentIndex() == PageIndex::LISTSET_SMALL) {
            if (stackPage->currentIndex() == PageIndex::MAINWINDOW_SMALL) {
                // disconnect mediaplayer's slot in MainWindow
                mainwindow->DisconnectMediaplayerEvent();
                mainwindowSmall->ConnectMediaplayerEvent();

                commonResrc->mediaPlayer_->setVideoOutput(mainwindow->getVideoOutput());
                mainwindow->RefreshList();
                mainwindow->RenderTheme();
                stackPage->setCurrentIndex(PageIndex::MAINWINDOW);
            } else if (stackPage->currentIndex() == PageIndex::LISTSET_SMALL) {
                listset->RefreshList();
                listset->RenderTheme();
                stackPage->setCurrentIndex(PageIndex::LISTSET);
            }
        }
    } else {
        // from big to small
        if (stackPage->currentIndex() == PageIndex::MAINWINDOW || stackPage->currentIndex() == PageIndex::LISTSET) {
            if (stackPage->currentIndex() == PageIndex::MAINWINDOW) {
                // disconnect mediaplayer's slot in MainWindow_Small
                mainwindowSmall->DisconnectMediaplayerEvent();
                mainwindow->ConnectMediaplayerEvent();

                commonResrc->mediaPlayer_->setVideoOutput(mainwindowSmall->getVideoOutput());
                mainwindowSmall->RefreshList();
                mainwindowSmall->RenderTheme();
                stackPage->setCurrentIndex(PageIndex::MAINWINDOW_SMALL);
            } else if (stackPage->currentIndex() == PageIndex::LISTSET) {
                listsetSmall->RefreshList();
                listsetSmall->RenderTheme();
                stackPage->setCurrentIndex(PageIndex::LISTSET_SMALL);
            }
        }
    }
}

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
        if (pageIndex == PageIndex::MAINWINDOW) {
            commonResrc->mediaPlayer_->setVideoOutput(mainwindow->getVideoOutput());
            mainwindow->RefreshList();
            mainwindow->Play();
        } else {
            commonResrc->mediaPlayer_->setVideoOutput(mainwindowSmall->getVideoOutput());
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
    QSize thresholdSize(600, 200000);

    if (size.width() >= thresholdSize.width()) {
        // from small to big
        if (stackPage->currentIndex() == PageIndex::MAINWINDOW_SMALL ||
            stackPage->currentIndex() == PageIndex::LISTSET_SMALL) {
            if (stackPage->currentIndex() == PageIndex::MAINWINDOW_SMALL) {
<<<<<<< HEAD
=======
                // disconnect mediaplayer's slot in MainWindow
                mainwindow->DisconnectMediaplayerEvent();
                mainwindowSmall->ConnectMediaplayerEvent();

>>>>>>> main
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
<<<<<<< HEAD
=======
                // disconnect mediaplayer's slot in MainWindow_Small
                mainwindowSmall->DisconnectMediaplayerEvent();
                mainwindow->ConnectMediaplayerEvent();

>>>>>>> main
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

#ifndef MAINWINDOWRESOURCE_H
#define MAINWINDOWRESOURCE_H

#include <QMediaPlayer>
#include <string>
#include <vector>

#include "fileutil.h"

// MainWindowResorce is the class to store
// common resource of mainwindow and mainwindow_mobile
class MainWindowResource {
public:
    MainWindowResource();
    ~MainWindowResource();

    QMediaPlayer* mediaPlayer_;
    int currentVideoIndex_{0};
    FileUtil* fileUtil_;
    std::vector<ListInfo> listinfo_;
    QStringList videoPaths_;
};

#endif  // MAINWINDOWRESOURCE_H

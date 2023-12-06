#include "btnconvert.h"

BtnConvert::BtnConvert(const QString &videoPath, QWidget *parent)
    : QPushButton(parent), videoPath(videoPath) {
}
QString BtnConvert::getVideoPath() const {
    return videoPath;
}

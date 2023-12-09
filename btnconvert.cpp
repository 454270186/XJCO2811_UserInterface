#include "btnconvert.h"
// Constructor for the BtnConvert class
BtnConvert::BtnConvert(const QString &videoPath, QWidget *parent)
    : QPushButton(parent), videoPath(videoPath) {
    // This is an initializer list used to initialize the QPushButton parent class and the videoPath member variable.
    // QPushButton(parent) calls the constructor of QPushButton, setting the parent of this button.
    // videoPath(videoPath) initializes the class's videoPath member variable with the videoPath argument passed to the constructor.
}

// The getVideoPath member function returns the video path associated with the button.
QString BtnConvert::getVideoPath() const {
    return videoPath;  // Returns the videoPath member variable stored in the object.
}



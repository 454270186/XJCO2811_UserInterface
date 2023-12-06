QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    btnconvert.cpp \
        fileutil.cpp \
        main.cpp \
        mainwindow.cpp \
        tinyxml2.cpp \
        listset.cpp \

HEADERS += \
    btnconvert.h \
    fileutil.h \
    mainwindow.h \
    tinyxml2.h \
    listset.h \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    listset.ui \
    mainwindow.ui


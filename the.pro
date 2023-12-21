QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    btnconvert.cpp \
    faq.cpp \
    fileutil.cpp \
    listsetresource.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowm.cpp \
    mainwindowresource.cpp \
    myvideowidget.cpp \
    pagemanager.cpp \
    tinyxml2.cpp \
    listset.cpp \
    formhandler.cpp \
    listsetsmall.cpp

HEADERS += \
    btnconvert.h \
    faq.h \
    fileutil.h \
    listsetresource.h \
    mainwindow.h \
    mainwindowm.h \
    mainwindowresource.h \
    myvideowidget.h \
    pagemanager.h \
    tinyxml2.h \
    tinyxml2.h \
    listset.h \
    formhandler.h \
    listsetsmall.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    faq.ui \
    listset.ui \
    mainwindow.ui \
    mainwindowm.ui \
    listsetsmall.ui

#ifndef UIMANAGEMENT_H
#define UIMANAGEMENT_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "mainwindow.h"
#include "mainwindowm.h"

#include "ui_mainwindow.h"
#include "ui_mainwindowm.h"

class UIManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit UIManagement(QWidget* parent = nullptr);
    ~UIManagement();

signals:
    // 新增的信号声明，用于通知窗口大小变化
    void resized();

private slots:
    void checkAndSwitchUI();

private:
    void initializeMainWindow();
    void initializeMainWindowM();
    void moveVideoWidgetTo(QWidget* targetUI);
    void resizeEvent(QResizeEvent* event) override;

    MainWindow* mainWindow;
    mainwindowm* mainWindowM;
    QMediaPlayer* mediaPlayer;
    QVideoWidget* videoWidget;
};

#endif // UIMANAGEMENT_H

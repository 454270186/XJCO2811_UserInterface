#include "uimanagement.h"
#include <QResizeEvent>

UIManagement::UIManagement(QWidget* parent)
    : QMainWindow(parent),
      mainWindow(new MainWindow),
      mainWindowM(new mainwindowm),
      mediaPlayer(new QMediaPlayer(this)),
      videoWidget(new QVideoWidget(this))
{
    // 初始化 MainWindow
    initializeMainWindow();

    // 设置视频播放器的输出
    mediaPlayer->setVideoOutput(videoWidget);

    // 初始时根据长宽比选择显示哪个窗口
    checkAndSwitchUI();
}

UIManagement::~UIManagement()
{
    delete mediaPlayer;
    delete videoWidget;
    delete mainWindow;
    delete mainWindowM;
}

void UIManagement::initializeMainWindow()
{
    // 从 mainwindow.ui 加载 UI
    mainWindow->ui->setupUi(mainWindow);  // 确保调用 setupUi
    // 添加其他用于初始化 MainWindow 的代码，比如连接信号和槽
}

void UIManagement::initializeMainWindowM()
{
    // 从 mainwindowm.ui 加载 UI
    mainWindowM->ui->setupUi(mainWindowM);  // 确保调用 setupUi
    // 添加其他用于初始化 MainWindowM 的代码，比如连接信号和槽
}

void UIManagement::moveVideoWidgetTo(QWidget* targetUI)
{
    // 将 videoWidget 从当前 UI 移动到目标 UI
    videoWidget->setParent(targetUI);
    videoWidget->show();
}

void UIManagement::resizeEvent(QResizeEvent* event)
{
    // 在窗口大小变化时调用
    QMainWindow::resizeEvent(event);

    // 发送 resized 信号，触发 checkAndSwitchUI
    emit resized();
}

void UIManagement::checkAndSwitchUI()
{
    // 根据宽高比切换窗口
    if (width() < height())
    {
        initializeMainWindowM();
        moveVideoWidgetTo(mainWindowM);
    }
    else
    {
        initializeMainWindow();
        moveVideoWidgetTo(mainWindow);
    }
}


//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <QApplication>
#include <QDesktopServices>
#include <QImageReader>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include "the_button.h"
#include "the_player.h"
#include <iostream>
#include <string>
#include <vector>

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn(std::string loc)
{
    std::cout << "123";
    std::vector<TheButtonInfo> out = std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc));
    QDirIterator it(dir);

    // 创建一个集合，用于跟踪已经生成的图片路径
    QSet<QString> generatedThumbnails;

    while (it.hasNext()) { // for all files
        std::cout << "asdas" << std::endl;
        QString f = it.next();

        if (f.contains("."))

#if defined(_WIN32)
        if (f.contains(".wmv") || f.contains(".mp4"))  { // windows
#else
        if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif
            std::cout << "sadsa";
            QString thumb = f.left(f.length() - 4) + ".png";

            // 检查是否已经生成相同路径的图片，如果是，则跳过
            if (generatedThumbnails.contains(thumb))
            {
                continue;
            }

            if (QFile(thumb).exists()) { // if a png thumbnail
                std::cout << "exist" << std::endl;
                QImageReader *imageReader = new QImageReader(thumb);
                QImage sprite = imageReader->read(); // read the thumbnail
                if (!sprite.isNull()) {
                    std::cout << "isNULL()" << std::endl;
                    QIcon *ico = new QIcon(
                        QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                    QUrl *url = new QUrl(
                        QUrl::fromLocalFile(f)); // convert the file location to a generic url
                    out.push_back(TheButtonInfo(url, ico)); // add to the output list
                    std::cout << "push_back" << std::endl;

                    // 将已生成的图片路径添加到集合中
                    generatedThumbnails.insert(thumb);
                } else {
                    qDebug() << "warning: skipping video because I couldn't process thumbnail "
                             << thumb << endl;
                }
            } else {
                qDebug() << "warning: skipping video because I couldn't find thumbnail "
                         << thumb << endl;
            }
        }
    }

    std::cout << "return" << std::endl;
    return out;
}

int main(int argc, char *argv[])
{
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2) {
        videos = getInfoIn(std::string(argv[1]));
        std::cout << argv[1] << "sbsbsb" << std::endl;
    } else {
        std::cout << argc << std::endl;
    }

    std::cout << "here:" << std::endl;
    if (videos.size() == 0) {
        std::cout << "here2:" << std::endl;
        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    std::cout << "here2:" << std::endl;
    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton *> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);

    std::cout << "here3:" << std::endl;
    // create the four buttons
    for (int i = 0; i < videos.size(); i++) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(buttonClicked(TheButtonInfo*)), player, SLOT(jumpTo(TheButtonInfo*)));
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    std::cout << "here4:" << std::endl;
    // tell the player what buttons and videos are available
    player->setContent(&buttons, &videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}

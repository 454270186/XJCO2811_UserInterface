#include "share.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>
#include "ui_share.h"

share::share(QWidget* parent) : QMainWindow(parent), ui(new Ui::share) {
    ui->setupUi(this);
    QFile file1("../XJCO2811_UserInterface/share_ch.qss");
    QString StyleSheet;
    if (file1.open(QFile::ReadOnly)) {
        StyleSheet += QLatin1String(file1.readAll());
        file1.close();
    } else {
        qDebug() << "File does not exist: " << file1.fileName();
    }

    if (!StyleSheet.isEmpty()) {
        this->setStyleSheet(StyleSheet);
    } else {
        qDebug() << "Current directory:" << QDir::currentPath();
    }

    // 在构造函数中确保 QLabel 控件存在
    ui->pictureone->show();
    ui->picturetwo->show();
    ui->picturethree->show();
    ui->picturefour->show();
    ui->picturefive->show();
    ui->picturesix->show();


    // 构建正确的图片路径并设置到 QLabel
    setPicture(ui->pictureone, "one.jpg");
    setPicture(ui->picturetwo, "two.jpg");
    setPicture(ui->picturethree, "three.jpg");
    setPicture(ui->picturefour, "four.jpg");
    setPicture(ui->picturefive, "five.jpg");
    setPicture(ui->picturesix, "six.jpg");

    connect(ui->shareone, &QPushButton::clicked, this, &share::onShareOneClicked);
    connect(ui->sharetwo, &QPushButton::clicked, this, &share::onShareTwoClicked);
    connect(ui->sharethree, &QPushButton::clicked, this, &share::onShareThreeClicked);
    connect(ui->sharefour, &QPushButton::clicked, this, &share::onShareFourClicked);
    connect(ui->sharefive, &QPushButton::clicked, this, &share::onShareFiveClicked);
    connect(ui->sharesix, &QPushButton::clicked, this, &share::onShareSixClicked);
}

share::~share() {
    delete ui;
}

void share::setPicture(QLabel* label, const QString& imageName) {
    // 构建相对路径
    QString relativePath = "../XJCO2811_UserInterface/picture/" + imageName;

    // 使用相对路径和应用程序的目录构建绝对路径
    QString imagePath = relativePath;

    // 加载图片
    QImage image(imagePath);
    if (image.isNull()) {
        qDebug() << "加载图片失败。错误信息: " << image.text() << " 文件路径：" << imagePath;
    } else {
        qDebug() << "成功加载图片!";

        // 调整图像大小
        QSize labelSize = label->size();
        QImage scaledImage = image.scaled(labelSize, Qt::KeepAspectRatio);

        // 设置图片到 QLabel
        label->setPixmap(QPixmap::fromImage(scaledImage));
    }
}

void share::onShareOneClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享失败", "赵老师觉得你太菜了!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::onShareTwoClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享失败", "因为子豪把你拉黑了!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::onShareThreeClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "大飞老师觉得你做得很好!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::onShareFourClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "德克王决定用你的分享打胶!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::onShareFiveClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "hy觉得你做得很好!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::onShareSixClicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享失败", "bi666正在跟男朋友过圣诞没空!");

    // 添加事件循环，确保消息框的事件被处理
    QCoreApplication::processEvents();
}

void share::loadStyleSheet(const QString& sheetName) {
    QFile file("../XJCO2811_UserInterface/" + sheetName);
    QString StyleSheet;
    if (file.open(QFile::ReadOnly)) {
        StyleSheet += QLatin1String(file.readAll());
        file.close();
    } else {
        qDebug() << "File does not exist: " << file.fileName();
    }

    if (!StyleSheet.isEmpty()) {
        this->setStyleSheet(StyleSheet);
    } else {
        qDebug() << "Failed to load stylesheet: " << sheetName;
    }
}

#include "share.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>
#include "ui_share.h"

share::share(QWidget* parent) : QMainWindow(parent), ui(new Ui::share) {
    ui->setupUi(this);

    // 在构造函数中确保 QLabel 控件存在
    ui->pictureone->show();
    ui->picturetwo->show();
    ui->picturethree->show();
    ui->picturefour->show();
    ui->picturefive->show();
    ui->picturesix->show();

    // 构建正确的图片路径并设置到 QLabel
    //    setPicture(ui->pictureone, "one.jpg");
    //    setPicture(ui->picturetwo, "two.jpg");
    //    setPicture(ui->picturethree, "three.jpg");
    //    setPicture(ui->picturefour, "four.jpg");
    //    setPicture(ui->picturefive, "five.jpg");
    //    setPicture(ui->picturesix, "six.jpg");

    connect(ui->shareone, &QPushButton::clicked, this, &share::on_shareone_clicked);
    connect(ui->sharetwo, &QPushButton::clicked, this, &share::on_sharetwo_clicked);
    connect(ui->sharethree, &QPushButton::clicked, this, &share::on_sharethree_clicked);
    connect(ui->sharefour, &QPushButton::clicked, this, &share::on_sharefour_clicked);
    connect(ui->sharefive, &QPushButton::clicked, this, &share::on_sharefive_clicked);
    connect(ui->sharesix, &QPushButton::clicked, this, &share::on_sharesix_clicked);
    connect(ui->exit, &QPushButton::clicked, this, &share::on_exit_clicked);
}

share::~share() {
    delete ui;
}

void share::setPicture(QLabel* label, const QString& imageName) {
    // 使用绝对路径
    QString imagePath = "D:/OneDrive - University of Leeds/Documents/XJCO2811_UserInterface/picture/" + imageName;

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

void share::on_shareone_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_sharetwo_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_sharethree_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_sharefour_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_sharefive_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_sharesix_clicked() {
    // 显示分享成功的弹窗
    QMessageBox::information(this, "分享成功", "您的分享已成功!");
}

void share::on_exit_clicked() {
    this->close();
}

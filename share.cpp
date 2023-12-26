#include "share.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QPixmap>
#include "ui_share.h"

#include <iostream>

share::share(QWidget* parent) : QMainWindow(parent), ui(new Ui::share) {
    ui->setupUi(this);

    ui->pictureone->show();
    ui->picturetwo->show();
    ui->picturethree->show();
    ui->picturefour->show();
    ui->picturefive->show();
    ui->picturesix->show();

    // laod avatar
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
    QString relativePath = "../XJCO2811_UserInterface/picture/" + imageName;

    // load image
    QImage image(relativePath);
    if (image.isNull()) {
        qDebug() << "Error: " << image.text();
    } else {
        QSize labelSize = label->size();
        QImage scaledImage = image.scaled(labelSize, Qt::KeepAspectRatio);

        label->setPixmap(QPixmap::fromImage(scaledImage));
    }
}

void share::onShareOneClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Mr.Zhao");

    QCoreApplication::processEvents();
}

void share::onShareTwoClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Yu");

    QCoreApplication::processEvents();
}

void share::onShareThreeClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Deck");

    QCoreApplication::processEvents();
}

void share::onShareFourClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Yue");

    QCoreApplication::processEvents();
}

void share::onShareFiveClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Zihao");

    QCoreApplication::processEvents();
}

void share::onShareSixClicked() {
    QMessageBox::information(this, "Share successfully", "Sent the screenshot to Biliu");

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

void share::RenderTheme(bool isChinese) {
    if (isChinese) {
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
    } else {
        QFile file1("../XJCO2811_UserInterface/share.qss");
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
    }
}

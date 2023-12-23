#include <QDebug>
#include <QDir>
#include <QFile>
#include <QResizeEvent>

#include "faq.h"
#include "listset.h"
#include "ui_faq.h"

Faq::Faq(QWidget* parent) : QDialog(parent), ui(new Ui::Faq) {
    ui->setupUi(this);
    QFile file1("../XJCO2811_UserInterface/faq.qss");
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

    connect(ui->backward, &QPushButton::clicked, this, &Faq::switchToPage);
    connect(ui->language, &QPushButton::clicked, this, &Faq::toggleLanguage);
    QResizeEvent resizeEvent(this->size(), QSize());
    this->resizeEvent(&resizeEvent);
}

Faq::~Faq() {
    delete ui;
}

// resizeEvent() handles the resizing of the FAQ dialog window. It performs the following steps:
// 1. Clears the existing layout items to prepare for reconfiguration.
// 2. Checks the new width of the window from the resize event.
// 3. If the width is less than or equal to 460, it arranges all questions and answers in a single column, setting the second column's stretch factor to 0.
// 4. If the width is greater than 460, it arranges the questions and answers in two columns, setting the second column's stretch factor to 1 to use available space.
// Params:
// - event: A QResizeEvent pointer that contains information about the resize event such as the new size of the window.
void Faq::resizeEvent(QResizeEvent* event) {
    QDialog::resizeEvent(event);

    int currentWidth = event->size().width();

    while (QLayoutItem* item = ui->gridLayout->takeAt(0)) {
        ui->gridLayout->removeItem(item);
    }

    if (currentWidth <= 460) {
        for (int i = 0; i < 8; ++i) {
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("question%1").arg(i + 1)), i * 2, 0);
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("answer%1").arg(i + 1)), i * 2 + 1, 0);
        }

        ui->gridLayout->setColumnStretch(1, 0);
    } else {
        for (int i = 0; i < 4; ++i) {
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("question%1").arg(i + 1)), i * 2, 0);
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("answer%1").arg(i + 1)), i * 2 + 1, 0);
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("question%1").arg(i + 5)), i * 2, 1);
            ui->gridLayout->addWidget(findChild<QLabel*>(QString("answer%1").arg(i + 5)), i * 2 + 1, 1);
        }

        ui->gridLayout->setColumnStretch(1, 1);
    }
}

// keyPressEvent() handles various keyboard events within the window.
// It performs specific actions based on the key pressed:
// - Qt::Key_Escape: Triggers switchToPage() if the backward button is enabled and visible.
// - Qt::Key_F2: Triggers switchToPage1() regardless of any conditions.
// Other keys are handled by the default QDialog keyPressEvent handler.
void Faq::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            if (ui->backward->isEnabled() && ui->backward->isVisible()) {
                switchToPage();
            }
            break;
        case Qt::Key_CapsLock:
            toggleLanguage();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

void Faq::toggleLanguage() {
    isChineseLanguage = !isChineseLanguage;
    QString sheetName = isChineseLanguage ? "faq_ch.qss" : "faq.qss";
    loadStyleSheet(sheetName);
}

void Faq::loadStyleSheet(const QString& sheetName) {
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

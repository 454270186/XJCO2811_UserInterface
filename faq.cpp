#include <QResizeEvent>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "faq.h"
#include "ui_faq.h"
#include "listset.h"

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
void Faq::resizeEvent(QResizeEvent *event) {
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

// switchToListset() is called to switch to the ListSet window.
// It hides the current MainWindow and shows a new ListSet window.
void Faq::switchToListset() {
    hide();
    ListSet* listsetWindow = new ListSet();
    listsetWindow->show();
}
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QResizeEvent>

#include "faq.h"
#include "listset.h"
#include "ui_faq.h"

Faq::Faq(QWidget* parent, ListSetResource* cr) : QDialog(parent), ui(new Ui::Faq), isSpeaking(false) {
    ui->setupUi(this);
    speech = new QTextToSpeech(this);
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

    // init common resource
    commonResrc = cr;

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
// - Qt::Key_Escape: Triggers switchToPage() if the backward button is enabled.
// - Qt::Key_F2: Triggers switchToPage() regardless of any conditions.
// - Qt::Key_Space: Activates the textToSpeech() method.
// Other keys are handled by the default QDialog keyPressEvent handler.
void Faq::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            if (ui->backward->isEnabled()) {
                switchToPage();
            }
            break;
        case Qt::Key_CapsLock:
            toggleLanguage();
            break;
        case Qt::Key_Space:
            textToSpeech();
            break;
        default:
            QDialog::keyPressEvent(event);
    }
}

// textToSpeech() manages the text-to-speech functionality for the FAQ dialog.
// It performs the following actions:
// - If text-to-speech is not currently speaking (isSpeaking is false):
//   1. Collects the text from all question and answer labels in the FAQ.
//   2. Concatenates them into a single string, separating questions and answers with a newline.
//   3. Starts speaking the concatenated text using the QTextToSpeech engine.
//   4. Sets the isSpeaking flag to true, indicating that speaking has started.
// - If text-to-speech is already active (isSpeaking is true):
//   1. Checks the current state of the QTextToSpeech engine.
//   2. If it is paused, resumes speaking.
//   3. If it is currently speaking, pauses the speech.
void Faq::textToSpeech() {
    if (!isSpeaking) {
        QString textToRead;
        for (int i = 1; i <= 8; ++i) {
            QLabel* questionLabel = findChild<QLabel*>(QString("question%1").arg(i));
            QLabel* answerLabel = findChild<QLabel*>(QString("answer%1").arg(i));
            if (questionLabel && answerLabel) {
                textToRead += questionLabel->text() + " " + answerLabel->text() + "\n";
            }
        }
        speech->say(textToRead);
        isSpeaking = true;
    } else {
        if (speech->state() == QTextToSpeech::Paused) {
            speech->resume();
        } else {
            speech->pause();
        }
    }
}

// stopReadingAndReset() stops any ongoing text-to-speech reading and resets speaking state.
// It performs the following actions:
// - Checks the current state of the QTextToSpeech engine.
// - If the engine is either speaking or paused, it stops the speech.
// - Resets the isSpeaking flag to false, indicating that no speech is currently active.
// This function is called when FAQ dialog is closed or when user navigates away from FAQ page.
void Faq::stopReadingAndReset() {
    if (speech->state() == QTextToSpeech::Speaking || speech->state() == QTextToSpeech::Paused) {
        speech->stop();
    }
    isSpeaking = false;
}

// switchToPage() is responsible for handling the navigation to a different page in the application.
// It performs the following actions:
// - Emits the stopReading signal to stop any ongoing text-to-speech reading in the FAQ page.
// - Emits the switchPage signal to navigate to a specific page.
void Faq::switchToPage() {
    emit stopReading();
    emit switchPage(1);
}

// toggleLanguage() switches the language and text-to-speech engine between Chinese and English.
// It performs the following actions:
// - Stops any ongoing text-to-speech reading and resets the speech state.
// - Deletes the existing QTextToSpeech instance and creates a new one for the selected language.
// - Toggles the language flag to switch between Chinese and English.
// - Loads the appropriate stylesheet based on the selected language.
// - Attempts to load the language-specific translation file. If successful, installs the translator to the application.
//   If the loading fails, logs an error message.
// - Updates the user interface to reflect the changes in language.
// This function is typically connected to a language toggle button.
void Faq::toggleLanguage() {
    stopReadingAndReset();
    delete speech;
    speech = new QTextToSpeech(this);

    commonResrc->isChineseLanguage_ = !commonResrc->isChineseLanguage_;
    QString sheetName = commonResrc->isChineseLanguage_ ? "faq_ch.qss" : "faq.qss";
    loadStyleSheet(sheetName);

    if (commonResrc->isChineseLanguage_) {
        if (translator.load("../XJCO2811_UserInterface/Faq_CN.qm")) {
            qApp->installTranslator(&translator);
        } else {
            qDebug() << "Failed to load translation file.";
        }
    } else {
        qApp->removeTranslator(&translator);
    }

    ui->retranslateUi(this);
}

// loadStyleSheet() loads a specific stylesheet file and applies it to the FAQ dialog.
// It performs the following actions:
// - Attempts to open the specified stylesheet file.
// - If the file opens successfully, reads its contents into a string.
// - Closes the file after reading its contents.
// - If the stylesheet string is not empty, applies it to the current instance of the dialog.
// - If the file does not exist or fails to load, logs an appropriate error message.
// This function is used to change appearance of FAQ dialog based on different themes or languages.
// Params:
// - sheetName: A QString representing the name of the stylesheet file to be loaded.
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

// RenderTheme() updates the FAQ dialog's appearance and language.
// It performs the following actions:
// - Checks the current language setting from the common resource.
// - Loads the appropriate stylesheet for either the Chinese or English language interface.
// - Attempts to load the language-specific translation file for Chinese.
//   If successful, installs the translator to the application to switch the interface language to Chinese.
//   If the loading fails, logs an error message.
// - If the language setting is not Chinese, removes any installed translator, reverting the interface language to English.
// This function is typically called when the FAQ dialog needs to update its appearance due to a change in language or theme settings.
void Faq::RenderTheme() {
    // check language and theme
    if (commonResrc->isChineseLanguage_) {
        loadStyleSheet("faq_ch.qss");
        if (translator.load("../XJCO2811_UserInterface/Faq_CN.qm")) {
            qApp->installTranslator(&translator);
        } else {
            qDebug() << "Failed to load translation file.";
        }
    } else {
        loadStyleSheet("faq.qss");
        qApp->removeTranslator(&translator);
    }

    ui->retranslateUi(this);
}

#ifndef FAQ_H
#define FAQ_H

#include <QDialog>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTranslator>
#include <QWidget>
#include <QTextToSpeech>

#include "listsetresource.h"

namespace Ui {
class Faq;
}

class Faq : public QDialog {
    Q_OBJECT

public:
    explicit Faq(QWidget* parent = nullptr, ListSetResource* cr = nullptr);
    ~Faq();

    void RenderTheme();
    void stopReadingAndReset();

signals:
    void switchPage(int pageIndex);
    void stopReading();

public slots:
    void toggleLanguage();

private slots:
    void switchToPage();

private:
    Ui::Faq* ui;
    QTextToSpeech *speech;
    QTranslator translator;
    ListSetResource* commonResrc;

    bool isChineseLanguage;
    bool isSpeaking;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void loadStyleSheet(const QString& sheetName);
    void textToSpeech();
};

#endif  // FAQ_H

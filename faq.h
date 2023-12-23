#ifndef FAQ_H
#define FAQ_H

#include <QDialog>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class Faq;
}

class Faq : public QDialog {
    Q_OBJECT

public:
    explicit Faq(QWidget* parent = nullptr);
    ~Faq();

signals:
    void switchPage(int pageIndex);

public slots:
    void toggleLanguage();

private slots:
    void switchToPage() { emit switchPage(1); }

private:
    Ui::Faq* ui;
    void resizeEvent(QResizeEvent* event) override;
    bool isChineseLanguage;
    void loadStyleSheet(const QString& sheetName);
    void keyPressEvent(QKeyEvent* event);
};

#endif  // FAQ_H

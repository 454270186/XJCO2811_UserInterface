#ifndef FAQ_H
#define FAQ_H

#include <QDialog>
#include <QListWidgetItem>
#include <QPushButton>

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
    void switchToListset();

private slots:
    void switchToPage() { emit switchPage(1); }

private:
    Ui::Faq* ui;
    void resizeEvent(QResizeEvent* event) override;
};

#endif  // FAQ_H
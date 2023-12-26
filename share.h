#ifndef SHARE_H
#define SHARE_H

#include <QLabel>
#include <QMainWindow>

namespace Ui {
class share;
}

class share : public QMainWindow {
    Q_OBJECT

public:
    explicit share(QWidget* parent = nullptr);
    ~share();

    void RenderTheme(bool isChinese);

private slots:
    void onShareOneClicked();
    void onShareTwoClicked();
    void onShareThreeClicked();
    void onShareFourClicked();
    void onShareFiveClicked();
    void onShareSixClicked();

private:
    Ui::share* ui;
    void setPicture(QLabel* label, const QString& imageName);
    void loadStyleSheet(const QString& sheetName);
};

#endif  // SHARE_H

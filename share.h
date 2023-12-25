#ifndef SHARE_H
#define SHARE_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class share;
}

class share : public QMainWindow {
    Q_OBJECT

public:
    explicit share(QWidget *parent = nullptr);
    void RenderTheme();
    ~share();

private slots:
    void onShareOneClicked();

    void onShareTwoClicked();

    void onShareThreeClicked();

    void onShareFourClicked();

    void onShareFiveClicked();

    void onShareSixClicked();


private:
    Ui::share *ui;
    void setPicture(QLabel *label, const QString &imageName);
    void loadStyleSheet(const QString& sheetName);
};

#endif // SHARE_H

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
    ~share();

private slots:
    void onShareOneClicked();

    void onShareTwoClicked();

    void onShareThreeClicked();

    void onShareFourClicked();

    void onShareFiveClicked();

    void onShareSixClicked();


private:
    void setPicture(QLabel *label, const QString &imageName);
    Ui::share *ui;
};

#endif // SHARE_H

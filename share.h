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
    void on_shareone_clicked();

    void on_sharetwo_clicked();

    void on_sharethree_clicked();

    void on_sharefour_clicked();

    void on_sharefive_clicked();

    void on_sharesix_clicked();

    void on_exit_clicked();

private:
    void setPicture(QLabel *label, const QString &imageName);
    Ui::share *ui;
};

#endif // SHARE_H

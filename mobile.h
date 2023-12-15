#ifndef MOBILE_H
#define MOBILE_H

#include <QMainWindow>

namespace Ui {
class mobile;
}

class mobile : public QMainWindow {
    Q_OBJECT

public:
    explicit mobile(QWidget* parent = nullptr);
    ~mobile();

private:
    Ui::mobile* ui;
};

#endif  // MOBILE_H

#ifndef LISTSET_H
#define LISTSET_H

#include <QMainWindow>

namespace Ui {
class ListSet;
}

class ListSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListSet(QWidget *parent = nullptr);
    ~ListSet();

private slots:
    void onSubmitClicked();

private:
    Ui::ListSet *ui;
};

#endif //LISTSET_H

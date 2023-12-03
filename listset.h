#ifndef LISTSET_H
#define LISTSET_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QVector>

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
    void on_pushButton_3_clicked();

private:
    Ui::ListSet *ui;
    QVBoxLayout *listLayout;
    QVector<QPushButton*> itemList;
    int clickCount;
};

#endif // LISTSET_H

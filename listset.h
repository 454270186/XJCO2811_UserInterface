#ifndef LISTSET_H
#define LISTSET_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QVector>

#include "fileutil.h"

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
    int on_addList_clicked();
    void onSubmitClicked();

private:
    FileUtil* fileUtil;
    std::vector<ListInfo> listsInfo;
    Ui::ListSet *ui;
    QVBoxLayout *listLayout;
    QPushButton *newButton;
    QVector<QPushButton*> itemList;
    int clickCount;
    int buttonClicked;
    QPushButton* currentButton;
};

#endif //LISTSET_H

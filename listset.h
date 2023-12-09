#ifndef LISTSET_H
#define LISTSET_H

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>

#include "fileutil.h"

namespace Ui {
class ListSet;
}

class ListSet : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSet(QWidget* parent = nullptr);

    bool buttonClicked = false;

    ~ListSet();

public slots:
    void switchToMainWindow();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();

private:
    FileUtil* fileUtil;
    std::vector<ListInfo> listsInfo;
    Ui::ListSet* ui;
    bool hasUnfinishedNewList = false;
    QVBoxLayout* listLayout;
    QPushButton* newButton;
    QVector<QPushButton*> itemList;
    int clickCount;
    QPushButton* currentButton;

    int currentBtnIndex{0};
};

#endif  //LISTSET_H

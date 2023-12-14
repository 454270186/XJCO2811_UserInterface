#ifndef LISTSETSMALL_H
#define LISTSETSMALL_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidgetItem>

#include <vector>

#include "fileutil.h" 

namespace Ui {
    class ListSetSmall;
}

class ListSetSmall : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSetSmall(QWidget* parent = nullptr);
    bool isSubmitEnabled = false;
    ~ListSetSmall();

public slots:
    void switchToMainWindow();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();

private:
    FileUtil* fileUtil;
    std::vector<ListInfo> listsInfo;
    Ui::ListSetSmall* ui;
    bool hasUnfinishedNewList = false;
    QHBoxLayout* listLayout; 
    QPushButton* newButton;
    QVector<QPushButton*> itemList;
    QPushButton* currentButton;
    int currentBtnIndex{0}; 
};

#endif  // LISTSETSMALL_H

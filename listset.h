#ifndef LISTSET_H
#define LISTSET_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidgetItem>

#include <vector>

#include "fileutil.h" 

namespace Ui {
    class ListSet;
}

class ListSet : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSet(QWidget* parent = nullptr);
    bool isSubmitEnabled = false;
    ~ListSet();

signals:
    void switchPage(int pageIndex);

public slots:
    void switchToMainWindow();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();
    void onDeleteClicked();
    void switchToPage() { emit switchPage(0); }

private:
    FileUtil* fileUtil;
    std::vector<ListInfo> listsInfo;
    Ui::ListSet* ui;
    bool hasUnfinishedNewList = false;
    QVBoxLayout* listLayout;
    QPushButton* newButton;
    QVector<QPushButton*> itemList;
    QPushButton* currentButton;
    int currentBtnIndex{0};
};

#endif  //LISTSET_H

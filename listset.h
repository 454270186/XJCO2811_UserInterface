#ifndef LISTSET_H
#define LISTSET_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <vector>
#include <map>
#include <QString>

#include "fileutil.h"

extern std::map<int, QString> errorMessages;

namespace Ui {
class ListSet;
}

class ListSet : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSet(QWidget* parent = nullptr);
    bool isSubmitEnabled = false;
    ~ListSet();

    void RefreshList();

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
    QVBoxLayout* listLayout;
    QPushButton* newButton;
    QVector<QPushButton*> itemList;
    QPushButton* currentButton;
    int currentBtnIndex{0};

    void renderList();
    void showError(int errorCode);

    bool hasUnfinishedNewList = false;
};

#endif  //LISTSET_H

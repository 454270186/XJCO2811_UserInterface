#ifndef LISTSETSMALL_H
#define LISTSETSMALL_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <vector>
#include <map>
#include <QString>

#include "fileutil.h"

extern std::map<int, QString> errorMessages;

namespace Ui {
    class ListSetSmall;
}

class ListSetSmall : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSetSmall(QWidget* parent = nullptr);
    bool isSubmitEnabled = false;
    ~ListSetSmall();

    void RefreshList();

public slots:
    void switchToMainWindow();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();
    void onDeleteClicked();

private:
    FileUtil* fileUtil;
    std::vector<ListInfo> listsInfo;
    Ui::ListSetSmall* ui;
    QHBoxLayout* listLayout;
    QPushButton* newButton;
    QVector<QPushButton*> itemList;
    QPushButton* currentButton;
    int currentBtnIndex{0};

    void renderList();
    void showError(int errorCode);

    bool hasUnfinishedNewList = false;
};

#endif  // LISTSETSMALL_H

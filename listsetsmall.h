#ifndef LISTSETSMALL_H
#define LISTSETSMALL_H

#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <map>
#include <vector>

#include "listsetresource.h"

#include <iostream>

extern std::map<int, QString> errorMessages;

namespace Ui {
class ListSetSmall;
}

class ListSetSmall : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSetSmall(QWidget* parent = nullptr, ListSetResource* cr = nullptr);
    bool isSubmitEnabled = false;
    ~ListSetSmall();

    void RefreshList();

signals:
    void switchPage(int pageIndex);

public slots:
    void switchToMainWindow();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();
    void onDeleteClicked();
    void onFindPathClicked();
    void switchToPage() {
        std::cout << "hereherehere" << std::endl;
        emit switchPage(2);
    }

private:
    Ui::ListSetSmall* ui;
    QHBoxLayout* listLayout;

    void renderList();
    void showError(int errorCode);

    ListSetResource* commonResrc;
};

#endif  // LISTSETSMALL_H

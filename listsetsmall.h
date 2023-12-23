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
    void toggleLanguage();

private slots:
    int on_addList_clicked();
    void onSubmitClicked();
    void onDeleteClicked();
    void onFindPathClicked();
    void switchToPage() { emit switchPage(2); }
    void switchToPage1() { emit switchPage(4); }

private:
    Ui::ListSetSmall* ui;
    QHBoxLayout* listLayout;

    void renderList();
    void showError(int errorCode);

    ListSetResource* commonResrc;
    bool isChineseLanguage;
    void loadStyleSheet(const QString& sheetName);
};

#endif  // LISTSETSMALL_H

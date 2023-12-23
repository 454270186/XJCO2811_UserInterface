#ifndef LISTSET_H
#define LISTSET_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <map>

#include "listsetresource.h"

extern std::map<int, QString> errorMessages;

namespace Ui {
class ListSet;
}

class ListSet : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSet(QWidget* parent = nullptr, ListSetResource* cr = nullptr);
    bool isSubmitEnabled = false;
    ~ListSet();

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
    void switchToPage() { emit switchPage(0); }
    void switchToPage1() { emit switchPage(4); }
    void onFindPathClicked();

private:
    Ui::ListSet* ui;
    QVBoxLayout* listLayout;

    void renderList();
    void showError(int errorCode);

    ListSetResource* commonResrc;
    bool isChineseLanguage;
    void loadStyleSheet(const QString &sheetName);
};

#endif  //LISTSET_H

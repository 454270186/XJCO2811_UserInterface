#ifndef LISTSET_H
#define LISTSET_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>

#include <map>
#include <vector>

#include "fileutil.h"
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

private slots:
    int on_addList_clicked();
    void onSubmitClicked();
    void onDeleteClicked();
    void switchToPage() { emit switchPage(0); }
    void onFindPathClicked();

private:
    Ui::ListSet* ui;
    QVBoxLayout* listLayout;

    void renderList();
    void showError(int errorCode);
    void keyPressEvent(QKeyEvent *event);

    ListSetResource* commonResrc;
};

#endif  //LISTSET_H

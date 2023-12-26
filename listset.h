#ifndef LISTSET_H
#define LISTSET_H

#include <map>

#include <QKeyEvent>
#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "listsetresource.h"

extern std::map<int, QString> errorMessagesEN;
extern std::map<int, QString> errorMessagesCN;

namespace Ui {
class ListSet;
}

class ListSet : public QMainWindow {
    Q_OBJECT

public:
    explicit ListSet(QWidget* parent = nullptr, ListSetResource* cr = nullptr);
    bool isSubmitEnabled = false;
    ~ListSet();

    // The public methods that are exposed to PageManager
    void RefreshList();
    void RenderTheme();

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
    void switchToPageFaq() { emit switchPage(4); }
    void onFindPathClicked();

private:
    Ui::ListSet* ui;

    // UI
    QVBoxLayout* listLayout;
    QLabel* labelName;
    QLabel* labelPath;
    // Data
    ListSetResource* commonResrc;
    std::map<int, QString> errorMessages;

    void renderList();
    void showError(int errorCode);
    void keyPressEvent(QKeyEvent* event);
    void loadStyleSheet(const QString& sheetName);
};

#endif  //LISTSET_H

#include <iostream>
#include <string>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "formhandler.h"
#include "listset.h"
#include "mainwindow.h"
#include "ui_listset.h"

ListSet::ListSet(QWidget* parent, ListSetResource* cr) : QMainWindow(parent), ui(new Ui::ListSet) {
    ui->setupUi(this);
    QFile file1("../XJCO2811_UserInterface/listset.qss");
    QString StyleSheet;
    if (file1.open(QFile::ReadOnly)) {
        StyleSheet += QLatin1String(file1.readAll());
        file1.close();
    } else {
        qDebug() << "File does not exist: " << file1.fileName();
    }

    if (!StyleSheet.isEmpty()) {
        this->setStyleSheet(StyleSheet);
    } else {
        qDebug() << "Current directory:" << QDir::currentPath();
    }

    listLayout = ui->scrollAreaWidget->findChild<QVBoxLayout*>("verticalLayout_6");

    connect(ui->submit, &QPushButton::clicked, this, &ListSet::onSubmitClicked);
    connect(ui->Delete, &QPushButton::clicked, this, &ListSet::onDeleteClicked);

    // init common resource
    commonResrc = cr;

    // Set the input form to invisible at first time
    ui->groupBox_right->setVisible(false);
    ui->midline->setVisible(false);
    ui->Delete->setVisible(false);

    renderList();

    connect(ui->backward, &QPushButton::clicked, this, &ListSet::switchToPage);
    connect(ui->qa, &QPushButton::clicked, this, &ListSet::switchToPage1);
    connect(ui->findPath, &QPushButton::clicked, this, &ListSet::onFindPathClicked);
    connect(ui->language, &QPushButton::clicked, this, &ListSet::toggleLanguage);
}

ListSet::~ListSet() {
    delete ui;
}

void ListSet::renderList() {
    for (size_t i = 0; i < commonResrc->ListInfo_.size(); i++) {
        // initialize video list ui
        QPushButton* newButton = new QPushButton();
        newButton->setObjectName("newButton");
        newButton->setText(commonResrc->ListInfo_[i].name.c_str());
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);
        listLayout->addWidget(newButton);

        connect(newButton, &QPushButton::clicked, [this, newButton] {
            ui->groupBox_right->setVisible(true);
            ui->midline->setVisible(true);
            ui->Delete->setVisible(true);
            ui->submit->setText(QString("Edit"));
            isSubmitEnabled = false;
            int index = listLayout->indexOf(newButton) - 1;
            commonResrc->currentBtnIndex_ = index;
            // Check if the index is valid
            if (index != -1 && index < (int)this->commonResrc->ListInfo_.size()) {
                ListInfo info = this->commonResrc->ListInfo_[index];
                ui->editName->setText(QString::fromStdString(info.name));
                ui->editPath->setText(QString::fromStdString(info.videoDirPath));
            }
        });
    }
}

void ListSet::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Return:
            if (ui->submit->isEnabled()) {
                onSubmitClicked();
            }
            break;
        case Qt::Key_Shift: 
            if (ui->Delete->isEnabled() && ui->Delete->isVisible()) { 
                onDeleteClicked(); 
            }
            break;
        case Qt::Key_Escape:
            if (ui->backward->isEnabled() && ui->backward->isVisible()) {
                switchToPage();
            }
            break;
        case Qt::Key_F1:
            if (ui->addList->isEnabled()) {
                on_addList_clicked();
            }
        default:
            QMainWindow::keyPressEvent(event);
    }
}

// on_addList_clicked() handles the event when the "Add List" button is clicked.
// It performs the following steps:
// 1. Checks if there is already an unfinished new list being added.
// 2. If not, creates a new QPushButton labeled "New List" and adds it to the list layout.
// 3. Sets up a connection to handle the click event of the new button.
//    When clicked, it makes the input form visible, sets the submit button text to "Submit",
//    and clears any existing text in the name and path edit fields.
// 4. If there is an unfinished list, it shows a warning message.
// Returns:
// - int: Always returns 0. This return value is not currently used.
int ListSet::on_addList_clicked() {
    if (!commonResrc->hasUnfinishedNewList_) {
        QPushButton* newButton = new QPushButton("New List");
        newButton->setObjectName("newButton");
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);
        listLayout->addWidget(newButton);
        commonResrc->hasUnfinishedNewList_ = true;

        connect(newButton, &QPushButton::clicked, [this, newButton] {
            ui->groupBox_right->setVisible(true);
            ui->midline->setVisible(true);
            ui->Delete->setVisible(false);
            ui->submit->setText("Submit");
            ui->editName->setText("");
            ui->editPath->setText("");
            isSubmitEnabled = true;
        });
    } else {
        QMessageBox::warning(this, "Warning", "There is already a new list.");
    }

    return 0;
}

// onSubmitClicked() handles the click event of the submit button.
// It performs different actions based on the current state:
// - If editing an existing list (isSubmitEnabled is false):
//   1. Calls editForm() from FormHandler with the current list ID, name, and path.
//   2. If successful, updates the UI to reflect the changes and resets isSubmitEnabled.
//   3. If failed, displays an error message.
// - If adding a new list (isSubmitEnabled is true):
//   1. Calls submitForm() from FormHandler with the new list name and path.
//   2. If successful, adds a new button for the list to the UI and resets hasUnfinishedNewList.
//   3. Updates currentBtnIndex to the index of the new button.
//   4. If failed, displays an error message.
void ListSet::onSubmitClicked() {
    std::string listName = ui->editName->text().toStdString();
    std::string videoDirPath = ui->editPath->text().toStdString();

    FormHandler formHandler;

    if (!isSubmitEnabled) {
        int result =
            formHandler.editForm(commonResrc->ListInfo_[commonResrc->currentBtnIndex_].id, listName, videoDirPath);
        if (result == FORMHANDLER_ERROR::SUCCESS) {
            QMessageBox::information(this, "Success", "List edited successfully!\n");
            QPushButton* button =
                qobject_cast<QPushButton*>(listLayout->itemAt(commonResrc->currentBtnIndex_ + 1)->widget());
            if (button) {
                button->setText(listName.c_str());
                connect(button, &QPushButton::clicked, [this, listName, videoDirPath] {
                    ui->editName->setText(listName.c_str());
                    ui->editPath->setText(videoDirPath.c_str());
                    isSubmitEnabled = false;
                });
            }
        } else {
            showError(result);
        }
    } else {
        int result = formHandler.submitForm(listName, videoDirPath);
        if (result == FORMHANDLER_ERROR::SUCCESS) {
            commonResrc->hasUnfinishedNewList_ = false;
            QMessageBox::information(this, "Success", "List added successfully!\n");
            QPushButton* newButton = new QPushButton(QString::fromStdString(listName));
            newButton->setObjectName("newButton");
            newButton->setCheckable(true);
            newButton->setAutoExclusive(true);
            listLayout->addWidget(newButton);
            commonResrc->fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
            commonResrc->ListInfo_ = commonResrc->fileUtil_->GetAllListsInfo();

            // Remove any existing "New List" button
            for (int i = 0; i < listLayout->count(); ++i) {
                QWidget* widget = listLayout->itemAt(i)->widget();
                if (widget) {
                    QPushButton* button = qobject_cast<QPushButton*>(widget);
                    if (button && button->text() == "New List") {
                        listLayout->removeWidget(button);
                        delete button;
                        break;
                    }
                }
            }

            // Connect new button and update currentBtnIndex
            connect(newButton, &QPushButton::clicked, [this, listName, videoDirPath, newButton] {
                ui->editName->setText(listName.c_str());
                ui->editPath->setText(videoDirPath.c_str());
                ui->Delete->setVisible(true);
                ui->submit->setText(QString("Edit"));
                isSubmitEnabled = false;
                commonResrc->currentBtnIndex_ = listLayout->indexOf(newButton) - 1;
            });
        } else {
            showError(result);
        }
    }
}

// onDeleteClicked() handles the click event of the delete button.
// It performs the following actions:
// - Checks if a valid list is currently selected (currentBtnIndex is within valid range).
//   If not, displays an error message and exits the function.
// - Calls DelListByID from FileUtil with the ID of the selected list.
// - If the deletion is successful (result > 0):
//   1. Displays a success message.
//   2. Removes the corresponding button from the UI.
//   3. Removes the list information from the listsInfo array.
//   4. Clears the interface when there is no list.
// - If the deletion fails (result <= 0), displays an error message.
void ListSet::onDeleteClicked() {
    if (commonResrc->currentBtnIndex_ < 0 || commonResrc->currentBtnIndex_ >= commonResrc->ListInfo_.size()) {
        QMessageBox::warning(this, "Error", "No list selected or invalid list index");
        return;
    }

    int listID = commonResrc->ListInfo_[commonResrc->currentBtnIndex_].id;
    string error;
    int result = commonResrc->fileUtil_->DelListByID(listID, &error);

    if (result == FORMHANDLER_ERROR::SUCCESS) {
        QMessageBox::information(this, "Success", "List deleted successfully");
        // Remove the corresponding button from the UI
        QWidget* widget = listLayout->itemAt(commonResrc->currentBtnIndex_ + 1)->widget();
        if (widget) {
            listLayout->removeWidget(widget);
            delete widget;
        }
        // Remove list information from listsInfo array
        commonResrc->ListInfo_.erase(commonResrc->ListInfo_.begin() + commonResrc->currentBtnIndex_);
        // Clear interface when there is no list
        commonResrc->ListInfo_.clear();
        commonResrc->ListInfo_ = commonResrc->fileUtil_->GetAllListsInfo();
        if (commonResrc->ListInfo_.empty()) {
            ui->groupBox_right->setVisible(false);
            ui->midline->setVisible(false);
        }
    } else {
        showError(result);
    }
}

// switchToMainWindow() handles the switch from the current ListSet window to the main window.
// It performs the following actions:
// 1. Hides the current ListSet window.
// 2. Creates a new instance of MainWindow.
// 3. Shows the MainWindow, allowing the user to interact with it.
void ListSet::switchToMainWindow() {
    hide();
    MainWindow* mainwindow = new MainWindow();
    mainwindow->show();
}

// RefreshList() updates and refreshes the list display in the ListSetSmall window.
// This function performs the following actions:
// - Checks if the listLayout pointer is null. If it is, it initializes listLayout
//   by finding the QHBoxLayout with the name "horizontalLayout_4" in the UI.
// - Clears the existing list buttons from the layout. It does this by repeatedly
//   removing and deleting the first layout item (and its associated widget) until
//   no items remain.
// - Clears the text in the edit fields for list name (editName) and path (editPath).
// - Retrieves updated list information by calling GetAllListsInfo from the FileUtil object.
// - Calls renderList() to display the updated list information in the UI.
// Parameters: None.
// Returns: None.
void ListSet::RefreshList() {
    if (listLayout == nullptr) {
        listLayout = ui->scrollAreaWidget->findChild<QVBoxLayout*>("verticalLayout_6");
    }

    // Clear existing buttons
    QLayoutItem* child;
    while ((child = listLayout->takeAt(1)) != nullptr) {
        delete child->widget();
        delete child;
    }

    ui->editName->setText("");
    ui->editPath->setText("");

    commonResrc->ListInfo_ = commonResrc->fileUtil_->GetAllListsInfo();
    renderList();
}

void ListSet::showError(int errorCode) {
    QString errorMsg = errorMessages.count(errorCode) ? errorMessages[errorCode] : errorMessages[0];
    QMessageBox::warning(this, "Error", errorMsg);
}

std::map<int, QString> errorMessages = {
    {FORMHANDLER_ERROR::ErrEmptyFields, "Error: One or more fields are empty!\n"},
    {FORMHANDLER_ERROR::ErrListNameTooLong, "Error: List name is too long!\n"},
    {FORMHANDLER_ERROR::ErrInvalidListNameChars, "Error: List name contains invalid characters!\n"},
    {FORMHANDLER_ERROR::ErrVideoDirPathTooLong, "Error: Video directory path is too long!\n"},
    {FORMHANDLER_ERROR::ErrInvalidVideoDirPathFormat, "Error: Invalid video directory path format!\n"},
    {FORMHANDLER_ERROR::ErrListNameNotUnique, "Error: List name is not unique!\n"},
    {FORMHANDLER_ERROR::ErrUnexpect, "Error: Unexpected result of form handler!\n"},
    {ERROR::ErrXMLParserInit, "Error: Initializing XML parser went wrong!\n"},
    {ERROR::ErrXMLChangeSave, "Error: Saving changes to XML file went wrong!\n"},
    {ERROR::ErrInvalidXML, "Error: Invalid XML file format!\n"},
    {ERROR::ErrUnexpect, "Error: Unexpected result of file utility!\n"},
    {ERROR::ErrListIDNotFound, "Error: List ID not found!\n"},
    {0, "Error: Unexpected result of list set!\n"}  // Default error message
};

void ListSet::onFindPathClicked() {
    QString initialPath = QDir::currentPath();  // or set to another base path
    QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Choose video directory"), initialPath);

    if (!directoryPath.isEmpty()) {
        // Convert the selected directory path to a relative path
        QDir baseDir(QDir::currentPath());  // Change this to the desired base directory
        QString relativePath = baseDir.relativeFilePath(directoryPath);

        ui->editPath->setText(relativePath);
    }
}

void ListSet::toggleLanguage() {
    isChineseLanguage = !isChineseLanguage;
    QString sheetName = isChineseLanguage ? "listset_ch.qss" : "listset.qss";
    loadStyleSheet(sheetName);
}

void ListSet::loadStyleSheet(const QString &sheetName) {
    QFile file("../XJCO2811_UserInterface/" + sheetName);
    QString StyleSheet;
    if (file.open(QFile::ReadOnly)) {
        StyleSheet += QLatin1String(file.readAll());
        file.close();
    } else {
        qDebug() << "File does not exist: " << file.fileName();
    }

    if (!StyleSheet.isEmpty()) {
        this->setStyleSheet(StyleSheet);
    } else {
        qDebug() << "Failed to load stylesheet: " << sheetName;
    }
}

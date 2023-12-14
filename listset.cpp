#include <iostream>
#include <string>

#include <QMessageBox>

#include "formhandler.h"
#include "listset.h"
#include "mainwindow.h"
#include "ui_listset.h"

ListSet::ListSet(QWidget* parent) : QMainWindow(parent), ui(new Ui::ListSet), hasUnfinishedNewList(false) {
    ui->setupUi(this);
    listLayout = ui->scrollAreaWidget->findChild<QVBoxLayout*>("verticalLayout_6");

    connect(ui->submit, &QPushButton::clicked, this, &ListSet::onSubmitClicked);

    // Load and process video list data from an XML file
    const std::string XMLFilePath = "../XJCO2811_UserInterface/videolist_data.xml";
    fileUtil = new FileUtil(XMLFilePath);
    listsInfo = fileUtil->GetAllListsInfo();

    // Set the input form to invisible at first time
    ui->groupBox_right->setVisible(false);
    ui->midline->setVisible(false);

    for (size_t i = 0; i < listsInfo.size(); i++) {
        // initialize video list ui
        QPushButton* newButton = new QPushButton();
        newButton->setText(listsInfo[i].name.c_str());
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);
        listLayout->addWidget(newButton);

        connect(newButton, &QPushButton::clicked, [this, newButton] {
            ui->groupBox_right->setVisible(true);
            ui->midline->setVisible(true);
            ui->submit->setText(QString("Edit"));
            isSubmitEnabled = false;
            int index = listLayout->indexOf(newButton) - 1;
            currentBtnIndex = index;
            // Check if the index is valid
            if (index != -1 && index < (int)this->listsInfo.size()) {
                ListInfo info = this->listsInfo[index];
                ui->editName->setText(QString::fromStdString(info.name));
                ui->editPath->setText(QString::fromStdString(info.videoDirPath));
            }      
        });
    }

    connect(ui->backward, &QPushButton::clicked, this, &ListSet::switchToPage);
}

ListSet::~ListSet() {
    delete ui;
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
    if (!hasUnfinishedNewList) {
        QPushButton* newButton = new QPushButton("New List");
        newButton->setCheckable(true);
        newButton->setAutoExclusive(true);
        listLayout->addWidget(newButton);
        hasUnfinishedNewList = true;

        connect(newButton, &QPushButton::clicked, [this, newButton] {
            ui->groupBox_right->setVisible(true);
            ui->midline->setVisible(true);
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
        int result = formHandler.editForm(listsInfo[currentBtnIndex].id, listName, videoDirPath);
        if (result > 0) {
            QMessageBox::information(this, "Success", "List edited successfully!\n");
            QPushButton* button = qobject_cast<QPushButton*>(listLayout->itemAt(currentBtnIndex + 1)->widget());
            if (button) {
                button->setText(listName.c_str());
                connect(button, &QPushButton::clicked, [this, listName, videoDirPath] {
                    ui->editName->setText(listName.c_str());
                    ui->editPath->setText(videoDirPath.c_str());
                    isSubmitEnabled = false;
                });
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to edit list!\n");
        }
    } else {
        int result = formHandler.submitForm(listName, videoDirPath);
        if (result > 0) {
            hasUnfinishedNewList = false;
            QMessageBox::information(this, "Success", "List added successfully!\n");
            QPushButton* newButton = new QPushButton(QString::fromStdString(listName));
            newButton->setCheckable(true);
            newButton->setAutoExclusive(true);
            listLayout->addWidget(newButton);
            fileUtil = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
            listsInfo = fileUtil->GetAllListsInfo();

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
                ui->submit->setText(QString("Edit"));
                isSubmitEnabled = false;
                currentBtnIndex = listLayout->indexOf(newButton) - 1;
            });
        } else {
            QMessageBox::warning(this, "Error", "Failed to add list!\n");
        }
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

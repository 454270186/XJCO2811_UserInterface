#include <QMessageBox>
#include <QPushButton>
#include <iostream>

#include <QDebug>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>
#include "formhandler.h"
#include "listset.h"
#include "mainwindow.h"
#include "ui_listset.h"

ListSet::ListSet(QWidget* parent) : QMainWindow(parent), ui(new Ui::ListSet), hasUnfinishedNewList(false) {
    ui->setupUi(this);
    listLayout = ui->scrollAreaWidget->findChild<QVBoxLayout*>("verticalLayout_6");

    connect(ui->submit, &QPushButton::clicked, this, &ListSet::onSubmitClicked);

    const std::string XMLFilePath = "../XJCO2811_UserInterface/videolist_data.xml";
    fileUtil = new FileUtil(XMLFilePath);
    fileUtil->PrintAll();
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
            buttonClicked = false;

            int index = listLayout->indexOf(newButton) - 1;
            currentBtnIndex = index;
            // Check if the index is valid
            if (index != -1 && index < (int)this->listsInfo.size()) {
                // Retrieve the corresponding ListInfo
                ListInfo info = this->listsInfo[index];
                // Set the text of editName and editPath
                ui->editName->setText(QString::fromStdString(info.name));
                ui->editPath->setText(QString::fromStdString(info.videoDirPath));
            }
        });

        std::cout << listsInfo[i].name << std::endl;
    }

    // connect signal and slot for switch from listset to mainwindow
    connect(ui->backward, &QPushButton::clicked, this, &ListSet::switchToMainWindow);
}

ListSet::~ListSet() {
    delete ui;
}

// on_addList_clicked() handles the click event of the "Add List" button.
// It increments the clickCount, creates a new QPushButton with a label,
// and adds the button to the listLayout of the user interface.
// The label of the button is set to "Item " followed by the current clickCount.
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
            buttonClicked = true;
        });
    } else {
        QMessageBox::warning(this, "Warning", "There is already a new list.");
    }

    return 0;
}

// onSubmitClicked() handles the click event of the submit button.
// It retrieves the list name and video directory path from the user interface,
// validates the data using FormHandler, and attempts to add a new list.
// Upon successful addition, it shows a success message to the user;
// if the addition fails, it shows an error message.
void ListSet::onSubmitClicked() {
    std::string listName = ui->editName->text().toStdString();
    std::string videoDirPath = ui->editPath->text().toStdString();

    FormHandler formHandler;

    if (!buttonClicked) {
        int result = formHandler.editForm(listsInfo[currentBtnIndex].id, listName, videoDirPath);
        if (result > 0) {
            QMessageBox::information(this, "Success", "List edited successfully!\n");

            QPushButton* button = qobject_cast<QPushButton*>(listLayout->itemAt(currentBtnIndex+1)->widget());
            if (button) {
                button->setText(listName.c_str());
                //disconnect(button, &QPushButton::clicked, 0, 0);
                connect(button, &QPushButton::clicked, [this, listName, videoDirPath] {
                    ui->editName->setText(listName.c_str());
                    ui->editPath->setText(videoDirPath.c_str());
                    buttonClicked = false;
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

            // 更新currentBtnIndex为新添加的按钮的索引
            fileUtil = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
            listsInfo = fileUtil->GetAllListsInfo();

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

            connect(newButton, &QPushButton::clicked, [this, listName, videoDirPath, newButton]{
                ui->editName->setText(listName.c_str());
                ui->editPath->setText(videoDirPath.c_str());
                ui->submit->setText(QString("Edit"));
                buttonClicked = false;

                currentBtnIndex = listLayout->indexOf(newButton)-1;
            });
        } else {
            QMessageBox::warning(this, "Error", "Failed to add list!\n");
        }
    }
}

void ListSet::switchToMainWindow() {
    hide();
    MainWindow* mainwindow = new MainWindow();
    mainwindow->show();
}

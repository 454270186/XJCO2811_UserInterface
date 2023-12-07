#include <QPushButton> 
#include <QMessageBox>

#include "formhandler.h"
#include "listset.h"
#include "ui_listset.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <string>

ListSet::ListSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListSet)
{
    ui->setupUi(this);
    listLayout = ui->scrollAreaWidget->findChild<QVBoxLayout*>("verticalLayout_6");

    clickCount = 0;
    buttonClicked = -1;
    connect(ui->submit, &QPushButton::clicked, this, &ListSet::onSubmitClicked);

    const std::string XMLFilePath ="../XJCO2811_UserInterface/videolist_data.xml";
    fileUtil = new FileUtil(XMLFilePath);
    fileUtil->PrintAll();
    listsInfo = fileUtil->GetAllListsInfo();
}

ListSet::~ListSet()
{
    delete ui;
}

// on_addList_clicked() handles the click event of the "Add List" button.
// It increments the clickCount, creates a new QPushButton with a label,
// and adds the button to the listLayout of the user interface.
// The label of the button is set to "Item " followed by the current clickCount.
int ListSet::on_addList_clicked() {
    clickCount++;

    QPushButton *newButton = new QPushButton;
    newButton->setText("New List " + QString::number(clickCount));
    listLayout->addWidget(newButton);

    listsInfo = fileUtil->GetAllListsInfo();
    fileUtil->PrintAll();
    std::string error;
    fileUtil->AddNewList(("New List " + QString::number(clickCount)).toStdString(), "../..", &error);
    qDebug() << listsInfo.size();

    connect(newButton, &QPushButton::clicked, [this, newButton] {
        int index = listLayout->indexOf(newButton);
        buttonClicked = index;
        qDebug() << "Button clicked. Index: " << index;
        // Check if the index is valid
        if (index != -1 && index < this->listsInfo.size()) {
            // Retrieve the corresponding ListInfo
            ListInfo info = this->listsInfo[index];
            // Set the text of editName and editPath
            ui->editName->setText(QString::fromStdString(info.name));
            ui->editPath->setText(QString::fromStdString(info.videoDirPath));
        }
    });
    return clickCount;
}


// onSubmitClicked() handles the click event of the submit button.
// It retrieves the list name and video directory path from the user interface,
// validates the data using FormHandler, and attempts to add a new list.
// Upon successful addition, it shows a success message to the user;
// if the addition fails, it shows an error message.
void ListSet::onSubmitClicked() {
    std::string listName = ui->editName->text().toStdString();
    std::string videoDirPath = ui->editPath->text().toStdString();
    if (buttonClicked != -1) {
        // Update the existing list in listsInfo
        qDebug() << "enter!" << buttonClicked <<  listsInfo.size();
        if (buttonClicked != -1 && buttonClicked <= listsInfo.size()) {
            listsInfo[buttonClicked].name = listName;
            listsInfo[buttonClicked].videoDirPath = videoDirPath;
            QPushButton *buttonIndex = qobject_cast<QPushButton*>(listLayout->itemAt(buttonClicked)->widget());
            buttonIndex->setText(QString::fromStdString(listName));
            QMessageBox::information(this, "Success", "List added successfully!\n");
        }
        buttonClicked = -1;  // Reset buttonClicked flag
    } else {
        FormHandler formHandler;
        int result = formHandler.submitForm(listName, videoDirPath);
        if (result > 0) {
            QMessageBox::information(this, "Success", "List added successfully!\n");
        } else {
            QMessageBox::warning(this, "Error", "Failed to add list!\n");
        }
        clickCount++;
        QPushButton *newButton = new QPushButton;
        newButton->setText(QString::fromStdString(listName));
        listLayout->addWidget(newButton);
    }
}


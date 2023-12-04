#include <iostream>

#include <QPushButton> 
#include <QMessageBox>

#include "formhandler.h"
#include "listset.h"
#include "ui_listset.h"

ListSet::ListSet(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ListSet)
{
    ui->setupUi(this);

    connect(ui->submit, &QPushButton::clicked, this, &ListSet::onSubmitClicked);
}

ListSet::~ListSet()
{
    delete ui;
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
    int result = formHandler.submitForm(listName, videoDirPath);

    if (result > 0) {       
        QMessageBox::information(this, "Success", "List added successfully!\n");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to add list!\n");
    }
}

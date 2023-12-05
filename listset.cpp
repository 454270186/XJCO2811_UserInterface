#include "listset.h"
#include "ui_listset.h"
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>

ListSet::ListSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListSet)
{
    ui->setupUi(this);
    listLayout = ui->scrollAreaWidgetContents_2->findChild<QVBoxLayout*>("verticalLayout_6");

    clickCount = 0;
}

ListSet::~ListSet()
{
    delete ui;
}

void ListSet::on_pushButton_4_clicked()
{
    clickCount++;

    QPushButton *newButton = new QPushButton;
    newButton->setText("Item " + QString::number(clickCount));

    // Add the new button to the layout
    listLayout->addWidget(newButton);
    // listLayout->update();
}


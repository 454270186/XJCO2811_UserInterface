#include "listset.h"
#include "ui_listset.h"
#include <QListWidgetItem>
#include <QDebug>

ListSet::ListSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListSet)
{
    ui->setupUi(this);
    // 获取垂直布局
    listLayout = ui->scrollAreaWidgetContents->findChild<QVBoxLayout*>("verticalLayout_3");
    // 初始化按钮点击次数
    clickCount = 0;
}

ListSet::~ListSet()
{
    delete ui;
}

void ListSet::on_pushButton_3_clicked()
{
    // 每次点击按钮，增加点击次数
    clickCount++;

    // 创建新按钮并设置文本
    QPushButton *newButton = new QPushButton;
    newButton->setText("Item " + QString::number(clickCount));

    // 将新的按钮添加到布局中
    listLayout->insertWidget(listLayout->count(), newButton);
}

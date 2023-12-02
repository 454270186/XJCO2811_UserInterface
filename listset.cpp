#include "listset.h"
#include "ui_listset.h"

ListSet::ListSet(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ListSet)
{
    ui->setupUi(this);
}

ListSet::~ListSet()
{
    delete ui;
}

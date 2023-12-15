#include "mobile.h"
#include "ui_mobile.h"

mobile::mobile(QWidget* parent) : QMainWindow(parent), ui(new Ui::mobile) {
    ui->setupUi(this);
}

mobile::~mobile() {
    delete ui;
}

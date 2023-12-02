//
//
//

#include "the_button.h"

TheButton::TheButton(QWidget *parent) : QPushButton(parent)
{
    setIconSize(QSize(200, 110));
    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void TheButton::init(TheButtonInfo *i)
{
    setIcon(*(i->icon));
    info = i;
}

void TheButton::onClicked()
{
    emit buttonClicked(info);
}

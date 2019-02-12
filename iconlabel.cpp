#include "iconlabel.h"

#include <QDebug>

IconLabel::IconLabel(QWidget *parent)
    :QPushButton(parent)
{

}

bool IconLabel::IconLabel::event(QEvent *e)
{
    if (e->type() == QEvent::Drop) {
        return QPushButton::event(e);
    }
    return true;
}

void IconLabel::mousePressEvent(QMouseEvent *event)
{

}

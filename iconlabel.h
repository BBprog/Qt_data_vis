#ifndef ICONLABEL_H
#define ICONLABEL_H

#include <QEvent>
#include <QPushButton>

class IconLabel : public QPushButton {

public:
    IconLabel(QWidget *parent = nullptr);

protected:
    bool event (QEvent * e);
    void mousePressEvent(QMouseEvent *event);
};

#endif // ICONLABEL_H

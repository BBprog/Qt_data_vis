#ifndef CQTABLEWIDGETITEM_H
#define CQTABLEWIDGETITEM_H

#include <QTableWidget>

class MyTableWidgetItem : public QTableWidgetItem {
    public:
    /*
        bool operator <(const QTableWidgetItem &other) const
        {
            return text().toInt() < other.text().toInt();
        }
    */
};

#endif // CQTABLEWIDGETITEM_H

#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>
#include <QMouseEvent>
#include <QHoverEvent>

class Table : public QTableWidget
{
public:
    Table(QWidget *parent = nullptr);

    void loadFile(QString filename);

    void fillItem(int col, QString text, QColor color);
    void fillAuto();
    void reinitialize();

    bool findEmptyItem();
    void removeSelection();

    QImage toImage();
private:
    QColor Kblank = Qt::white;
    QList<QColor> col_priority;

    void init(int nbColumn, QStringList headerLabels);
};

#endif // TABLE_H

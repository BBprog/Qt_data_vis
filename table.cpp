#include "csvreader.h"
#include "table.h"

#include <QDebug>

Table::Table(QWidget *parent)
    : QTableWidget(parent)
{ }

void Table::init(int nbColumn, QStringList headerLabels)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);
    setSortingEnabled(true);

    setRowCount(0);
    setColumnCount(nbColumn);
    setHorizontalHeaderLabels(headerLabels);
}

void Table::loadFile(QString filename) {
    CSVReader *reader = new CSVReader(filename, ",;");

    QList<QList<QString> > data = reader->getData();

    init(data.first().length(), data.first());
    data.removeFirst();

    foreach(QList<QString> str, data) {
        int row = rowCount();
        int col = 0;

        insertRow(row);

        foreach(QString el, str) {
            QTableWidgetItem *item = new QTableWidgetItem;

            QRegExp reInt("\\d*");
            QRegExp reDouble("[+-]?\\d*\\.?\\d+");
            if (reInt.exactMatch(el)) {
                item->setData(Qt::EditRole, el.toInt());
            }
            else if (reDouble.exactMatch(el)) {
                item->setData(Qt::EditRole, el.toDouble());
            }
            else {
                item->setData(Qt::EditRole, el);
            }

            item->setTextAlignment(Qt::AlignRight);
            item->setBackgroundColor(Kblank);

            setItem(row, col, item);
            col++;
        }
    }
}

void Table::fillItem(int col, QString text, QColor color)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    selectColumn(col);
    foreach(QTableWidgetItem *item, selectedItems()) {
        if (item->text() == text)
            item->setBackgroundColor(color);
        else if (item->backgroundColor() == color) {
            item->setBackgroundColor(Kblank);
        }
    }

    removeSelection();
}

void Table::fillAuto()
{
    QMap<QString, QColor> index;
    QColor color;

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    for (int i = 0; i < columnCount(); ++i) {
        index.clear();
        selectColumn(i);
        foreach(QTableWidgetItem *item, selectedItems()) {
            if (item->backgroundColor().toHsv() == Kblank.toHsv()) {
                if (!index.contains(item->text())) {
                    color = QColor::fromHsv(qrand()%359, qrand()%255, 255);
                    index.insert(item->text(), color);
                }
                else {
                    color = index.value(item->text());
                }
                item->setBackgroundColor(color);
            }
        }
    }

    removeSelection();
}

bool Table::findEmptyItem()
{
    bool itemFound = false;
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    int i = 0;
    while (!itemFound && i < columnCount()) {
        selectColumn(i);
        foreach(QTableWidgetItem *item, selectedItems()) {
            if (item->backgroundColor() == Kblank) {
                itemFound = true;
                break;
            }
        }
        ++i;
    }

    removeSelection();

    return itemFound;
}

void Table::reinitialize()
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    selectAll();
    foreach(QTableWidgetItem *item, selectedItems()) {
        item->setBackgroundColor(Kblank);
    }
    removeSelection();
}

void Table::removeSelection()
{
    foreach(QTableWidgetSelectionRange selection, selectedRanges())
    {
        setRangeSelected(selection, false);
    }
    setSelectionMode(QAbstractItemView::NoSelection);
}

/*
QImage Table::toImage()
{
    QImage image = QImage(columnCount(), rowCount(), QImage::Format_RGB666);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < columnCount(); ++i) {
        selectColumn(i);
        foreach(QTableWidgetItem *item, selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    removeSelection();

    return image;
}
*/

QImage Table::toImage()
{
    QImage image = QImage(500, 500, QImage::Format_RGB666);
    for (int i = 0; i < 500; ++i) {
        for (int j = 0; j < 500; ++j) {
            image.setPixelColor(i, j, Qt::blue);
        }
    }
    return image;
}

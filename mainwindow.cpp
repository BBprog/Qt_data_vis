#include "mainwindow.h"
#include "csvreader.h"
#include "imageviewer.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QInputDialog>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    enableEditionTools(false);

    colorPicker = new QColorDialog(this);
    colorPicker->setOption(QColorDialog::NoButtons, true);
    colorPicker->setVisible(false);
    preview->setVisible(false);

    central->layout()->addWidget(colorPicker);

    /*
    QColor color = QColor::fromHsv(qrand()%359, qrand()%255, 255);

    int r, g, b, colorDec;

    color.getRgb(&r, &g, &b);

    qDebug() << color.red() << color.green() << color.blue();
    colorDec = r*g*b;
    int ratio = 255.0*255*255/10000;
    qDebug() << colorDec;
    colorDec = colorDec - colorDec%ratio;
    qDebug() << colorDec;

    QString result = "#" + QString::number( colorDec, 16 );
    qDebug() << result;
    color.setNamedColor(result);
    qDebug() << color.red() << color.green() << color.blue();
    */
}

void MainWindow::initTable(int nbColumn, QStringList headerLabels)
{
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setSortingEnabled(true);

    table->setRowCount(0);
    table->setColumnCount(nbColumn);
    table->setHorizontalHeaderLabels(headerLabels);
}

void MainWindow::enableEditionTools(bool active)
{
    menuEdit->setEnabled(active);
    mainToolBar->setEnabled(active);
    mainToolBar->setVisible(active);
}

void MainWindow::fillItem(int col, QString text, QColor color)
{
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    table->selectColumn(col);
    foreach(QTableWidgetItem *item, table->selectedItems()) {
        if (item->text() == text)
            item->setBackgroundColor(color);
        else if (item->backgroundColor() == color) {
            item->setBackgroundColor(Kblank);
        }
    }

    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

bool MainWindow::findEmptyItem()
{
    bool itemFound = false;
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    int i = 0;
    while (!itemFound && i < table->columnCount()) {
        table->selectColumn(i);
        foreach(QTableWidgetItem *item, table->selectedItems()) {
            if (item->backgroundColor() == Kblank) {
                itemFound = true;
                break;
            }
        }
        ++i;
    }

    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    return itemFound;
}

void MainWindow::updateImageView() {
    QImage image = QImage(table->columnCount(), table->rowCount(), QImage::Format_RGB16);

    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < table->columnCount(); ++i) {
        table->selectColumn(i);
        foreach(QTableWidgetItem *item, table->selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    imagePreview->setPixmap(QPixmap::fromImage(image));
    preview->adjustSize();
}

void MainWindow::errorMsgBox(QString msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Woups...");
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Load File"), "../", tr("CSV Files (*.csv)"));

    if (fileName.isEmpty()) {
        errorMsgBox("File not found");
        return;
    }

    CSVReader *reader = new CSVReader(fileName, ",;");

    QList<QList<QString> > data = reader->getData();

    if (data.isEmpty()) {
        errorMsgBox("Can't read data from file");
        return;
    }

    initTable(data.first().length(), data.first());
    data.removeFirst();

    foreach(QList<QString> str, data) {
        int row = table->rowCount();
        int col = 0;

        table->insertRow(row);

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

            table->setItem(row, col, item);
            col++;
        }
    }

    updateImageView();
    enableEditionTools(true);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void MainWindow::on_actionFill_auto_triggered()
{
    QMap<QString, QColor> index;
    QColor color;

    table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    for (int i = 0; i < table->columnCount(); ++i) {
        index.clear();
        table->selectColumn(i);
        foreach(QTableWidgetItem *item, table->selectedItems()) {
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

    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

void MainWindow::on_actionPick_color_toggled(bool active)
{
    colorPicker->setVisible(active);
}

void MainWindow::on_actionReinitialize_triggered()
{
    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    table->selectAll();
    foreach(QTableWidgetItem *item, table->selectedItems()) {
        item->setBackgroundColor(Kblank);
    }
    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

void MainWindow::on_actionFill_selected_triggered()
{
    if (!table->selectedItems().isEmpty()) {
        fillItem(table->selectedItems().first()->column(), table->selectedItems().first()->text(), colorPicker->currentColor());
    }
}

void MainWindow::on_actionGenerate_Image_triggered()
{
    /*
    int nbColumn = table->columnCount();

    bool ok;
    int i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                 tr("Nombre de colonnes"), 1, 1, table->rowCount()/table->columnCount(), 1, &ok);
    if (ok)
        nbColumn = i;
    */

    if (findEmptyItem()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                        "Certaines cases sont encore de couleur blanche. Souhaitez-vous lancer une complétion automatiquement du remplissage ?",
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            on_actionFill_auto_triggered();
        else if (reply == QMessageBox::Cancel)
            return;
    }

    QImage image = QImage(table->columnCount(), table->rowCount(), QImage::Format_RGB666);

    table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < table->columnCount(); ++i) {
        table->selectColumn(i);
        foreach(QTableWidgetItem *item, table->selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    table->setRangeSelected(table->selectedRanges().first(), false);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    /*
    QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Location"), "img.png", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    image.save(fileName);
    */

    imagePreview->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionShow_image_preview_triggered(bool checked)
{
    preview->setVisible(checked);
}

void MainWindow::on_actionFill_clicked_triggered(bool checked)
{
    brushActive = checked;
    colorPicker->setVisible(checked);
    (checked) ? setCursor(Qt::CrossCursor)
              : setCursor(Qt::ArrowCursor);
}

void MainWindow::on_table_itemClicked(QTableWidgetItem *item)
{
    if (brushActive) {
        fillItem(item->column(), item->text(), colorPicker->currentColor());
    }
}

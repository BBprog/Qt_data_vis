#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvreader.h"
#include "imageviewer.h"

#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QScreen>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    enableEditionTools(false);

    colorPicker = new QColorDialog(this);
    colorPicker->setOption(QColorDialog::NoButtons, true);
    colorPicker->setVisible(false);
    ui->preview->setVisible(false);

    ui->centralWidget->layout()->addWidget(colorPicker);

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTable(int nbColumn, QStringList headerLabels)
{
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table->setSortingEnabled(true);

    ui->table->setRowCount(0);
    ui->table->setColumnCount(nbColumn);
    ui->table->setHorizontalHeaderLabels(headerLabels);
}

void MainWindow::enableEditionTools(bool active)
{
    ui->menuEdit->setEnabled(active);
    ui->mainToolBar->setEnabled(active);
    ui->mainToolBar->setVisible(active);
}

void MainWindow::fillItem(int col, QString text, QColor color)
{
    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->table->selectColumn(col);
    foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
        if (item->text() == text)
            item->setBackgroundColor(color);
        else if (item->backgroundColor() == color) {
            item->setBackgroundColor(Kblank);
        }
    }

    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

bool MainWindow::findEmptyItem()
{
    bool itemFound = false;
    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    int i = 0;
    while (!itemFound && i < ui->table->columnCount()) {
        ui->table->selectColumn(i);
        foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
            if (item->backgroundColor() == Kblank) {
                itemFound = true;
                break;
            }
        }
        ++i;
    }

    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    return itemFound;
}

void MainWindow::updateImageView() {
    QImage image = QImage(ui->table->columnCount(), ui->table->rowCount(), QImage::Format_RGB16);

    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < ui->table->columnCount(); ++i) {
        ui->table->selectColumn(i);
        foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    ui->imagePreview->setPixmap(QPixmap::fromImage(image));
    ui->preview->adjustSize();
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
        int row = ui->table->rowCount();
        int col = 0;

        ui->table->insertRow(row);

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

            ui->table->setItem(row, col, item);
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

    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    for (int i = 0; i < ui->table->columnCount(); ++i) {
        index.clear();
        ui->table->selectColumn(i);
        foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
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

    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

void MainWindow::on_actionPick_color_toggled(bool active)
{
    colorPicker->setVisible(active);
}

void MainWindow::on_actionReinitialize_triggered()
{
    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->table->selectAll();
    foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
        item->setBackgroundColor(Kblank);
    }
    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    updateImageView();
}

void MainWindow::on_actionFill_selected_triggered()
{
    if (!ui->table->selectedItems().isEmpty()) {
        fillItem(ui->table->selectedItems().first()->column(), ui->table->selectedItems().first()->text(), colorPicker->currentColor());
    }
}

void MainWindow::on_actionGenerate_Image_triggered()
{
    /*
    int nbColumn = ui->table->columnCount();

    bool ok;
    int i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                 tr("Nombre de colonnes"), 1, 1, ui->table->rowCount()/ui->table->columnCount(), 1, &ok);
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

    QImage image = QImage(ui->table->columnCount(), ui->table->rowCount(), QImage::Format_RGB666);

    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < ui->table->columnCount(); ++i) {
        ui->table->selectColumn(i);
        foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::NoSelection);

    /*
    QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Location"), "img.png", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    image.save(fileName);
    */

    ui->imagePreview->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionShow_image_preview_triggered(bool checked)
{
    ui->preview->setVisible(checked);
}

void MainWindow::on_actionfill_clicked_triggered(bool checked)
{
    colorPicker->setVisible(true);
    brushActive = checked;
}

void MainWindow::on_table_itemClicked(QTableWidgetItem *item)
{
    if (brushActive) {
        fillItem(item->column(), item->text(), colorPicker->currentColor());
    }
}

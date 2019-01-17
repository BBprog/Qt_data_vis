#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvreader.h"
#include "cqtablewidgetitem.h"

#include <QFileDialog>
#include <QDebug>
#include <QStackedLayout>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    currentColor = Qt::white;

    ui->setupUi(this);
    enableEditionTools(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTable(int nbColumn, QStringList headerLabels)
{
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->table->setColumnCount(nbColumn);
    ui->table->setHorizontalHeaderLabels(headerLabels);
}

void MainWindow::enableEditionTools(bool active)
{
    ui->menuEdit->setEnabled(active);
    ui->mainToolBar->setEnabled(active);
}

void MainWindow::fillItem(int col, QString text, QColor color, int begin)
{
    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->table->selectColumn(col);

    foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
        if (item->text() == text)
            item->setBackgroundColor(color);
    }

    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
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

            ui->table->setItem(row, col, item);
            col++;
        }
    }

    ui->table->setSortingEnabled(true);

    enableEditionTools(true);
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

    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_actionPick_color_triggered()
{
    currentColor = QColorDialog::getColor(currentColor, this, QString());
    on_actionFill_selected_triggered();
}

void MainWindow::on_actionReinitialize_triggered()
{
    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->table->selectAll();
    foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
        item->setBackgroundColor(Qt::white);
    }
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table->setRangeSelected(ui->table->selectedRanges().first(), false);
}

void MainWindow::on_actionFill_selected_triggered()
{
    if (!ui->table->selectedItems().isEmpty()) {
        fillItem(ui->table->selectedItems().first()->column(), ui->table->selectedItems().first()->text(), currentColor);
    }
}

void MainWindow::on_actionGenerate_Image_triggered()
{
    QImage image = QImage(ui->table->columnCount(), ui->table->rowCount(), QImage::Format_RGB16);

    ui->table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    for (int i = 0; i < ui->table->columnCount(); ++i) {
        ui->table->selectColumn(i);
        foreach(QTableWidgetItem *item, ui->table->selectedItems()) {
            image.setPixelColor(item->column(), item->row(), item->backgroundColor());
        }
    }
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);

    QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Location"), "img.png", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    image.save(fileName);
}

void MainWindow::on_actionClose_triggered()
{
    QApplication::quit();
}

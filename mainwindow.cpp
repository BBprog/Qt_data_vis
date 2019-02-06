#include "mainwindow.h"
#include "imageviewer.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    init();

    enableEditionTools(false);

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

void MainWindow::init()
{
    table = new Table;
    table->setMinimumWidth(100);

    colorPicker = new QColorDialog(this);
    colorPicker->setOption(QColorDialog::NoButtons, true);
    colorPicker->setVisible(false);

    preview = new ImageViewer;
    preview->setMinimumHeight(100);
    preview->setVisible(false);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(preview);

    splitter = new QSplitter;
    splitter->addWidget(table);
    splitter->addWidget(preview);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);

    main->setLayout(new QHBoxLayout);
    main->layout()->addWidget(splitter);
    main->layout()->addWidget(colorPicker);

    main->setVisible(false);

    connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(on_table_itemClicked(QTableWidgetItem*)));
}

void MainWindow::enableEditionTools(bool active)
{
    menuEdit->setEnabled(active);
    mainToolBar->setEnabled(active);
    mainToolBar->setVisible(active);
}

void MainWindow::updateImageView() {
    QImage image = table->toImage();
    preview->loadImage(image);
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

    table->loadFile(fileName);

    updateImageView();
    enableEditionTools(true);

    accueil->setVisible(false);
    main->setVisible(true);
}

void MainWindow::on_actionFill_auto_triggered()
{
    table->fillAuto();
    updateImageView();
}

void MainWindow::on_actionPick_color_toggled(bool checked)
{
    colorPicker->setVisible(checked);
    splitter->adjustSize();
}

void MainWindow::on_actionReinitialize_triggered()
{
    table->reinitialize();
    updateImageView();
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

    if (table->findEmptyItem()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                        "Certaines cases sont encore de couleur blanche. Souhaitez-vous lancer une complétion automatiquement du remplissage ?",
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            on_actionFill_auto_triggered();
        else if (reply == QMessageBox::Cancel)
            return;
    }

    QImage image = table->toImage();

    /*
    QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Location"), "img.png", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    image.save(fileName);
    */

    preview->loadImage(image);
}

void MainWindow::on_actionClose_triggered()
{
    main->setVisible(false);
    accueil->setVisible(true);
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
        table->fillItem(item->column(), item->text(), colorPicker->currentColor());
    }
    updateImageView();
}

void MainWindow::on_pushButton_clicked()
{
    on_actionLoad_triggered();
}

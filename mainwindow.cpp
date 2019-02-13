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
    resize(2*QGuiApplication::primaryScreen()->availableSize().width()/3,
           2*QGuiApplication::primaryScreen()->availableSize().height()/3);

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
    table->setMinimumSize(200, 100);

    colorPicker = new QColorDialog(this);
    colorPicker->setOption(QColorDialog::NoButtons, true);
    colorPicker->setVisible(false);

    previewArea = new QScrollArea;
    previewArea->setAlignment(Qt::AlignCenter);
    previewArea->setMinimumSize(150, 100);

    preview = new ImageViewer(100);
    previewArea->setWidget(preview);

    splitter = new QSplitter;
    splitter->addWidget(table);
    splitter->addWidget(previewArea);
    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
    splitter->setChildrenCollapsible(false);

    main->setLayout(new QHBoxLayout);
    main->layout()->addWidget(colorPicker);
    main->layout()->addWidget(splitter);

    footerLabel = new QLabel;
    footer->addWidget(footerLabel);

    switchToEdit(false);

    connect(table, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(on_table_itemClicked(QTableWidgetItem*)));
}

void MainWindow::enableEditionTools(bool active)
{
    menuEdit->setEnabled(active);
    menuShow->setEnabled(active);
    mainToolBar->setEnabled(active);
    mainToolBar->setVisible(active);
    foreach(QAction *act, menuFile->actions())
    {
        if (act->objectName() == "actionSave_Image") {
            act->setEnabled(active);
        }
        else if (act->objectName() == "actionClose") {
            act->setEnabled(active);
        }
    }
}

void MainWindow::switchToEdit(bool active)
{
    enableEditionTools(active);
    main->setVisible(active);
    accueil->setVisible(!active);
    matchButtonsStyle();
}

void MainWindow::matchButtonsStyle()
{
    foreach(QAction * act, mainToolBar->actions())
    {
        if (act->objectName() == "actionShow_image_preview")
            act->setChecked(preview->isVisible());
        else if (act->objectName() == "actionPick_color")
            act->setChecked(colorPicker->isVisible());
    }
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

void MainWindow::writeStatusBar(QString text)
{
    footerLabel->setText(text);
}

void MainWindow::on_actionLoad_triggered()
{
    writeStatusBar("Chargement des données...");

    QString fileName = QFileDialog::getOpenFileName(this,
                        tr("Load File"), "../", tr("CSV Files (*.csv)"));

    if (fileName.isEmpty()) {
        errorMsgBox("File not found");
        writeStatusBar("");
        return;
    }

    table->loadFile(fileName);

    switchToEdit(true);

    updateImageView();

    QString taille = "Volume des données: "
            + QString::number(table->columnCount())
            + " x "
            + QString::number(table->rowCount());
    writeStatusBar(taille);
}

void MainWindow::on_actionFill_auto_triggered()
{
    table->fillAuto();
    updateImageView();
}

void MainWindow::on_actionPick_color_toggled(bool checked)
{
    colorPicker->setVisible(checked);
}

void MainWindow::on_actionReinitialize_triggered()
{
    table->reinitialize();
    updateImageView();
}

void MainWindow::on_actionSave_Image_triggered()
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

    QString filename = QFileDialog::getSaveFileName(this,
                tr("Save Location"), "img.png", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    image.save(filename);
}

void MainWindow::on_actionShow_image_preview_triggered(bool checked)
{
    previewArea->setVisible(checked);
}

void MainWindow::on_actionFill_clicked_triggered(bool checked)
{
    brushActive = checked;
    colorPicker->setVisible(checked);
    (checked) ? setCursor(Qt::CrossCursor)
              : setCursor(Qt::ArrowCursor);
    matchButtonsStyle();
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

void MainWindow::on_actionClose_triggered()
{
    writeStatusBar("");
    switchToEdit(false);
}

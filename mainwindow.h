#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageviewer.h"
#include "table.h"

#include <QColorDialog>
#include <QMainWindow>
#include <QSplitter>
#include <QScrollArea>
#include <ui_mainwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_actionLoad_triggered();

    void on_actionFill_auto_triggered();

    void on_actionReinitialize_triggered();

    void on_actionSave_Image_triggered();

    void on_actionPick_color_toggled(bool active);

    void on_actionShow_image_preview_triggered(bool checked);

    void on_actionFill_clicked_triggered(bool checked);

    void on_table_itemClicked(QTableWidgetItem *item);

    void on_actionClose_triggered();

    void on_pushButton_clicked();
private:
    QColorDialog *colorPicker;
    QScrollArea *previewArea;
    ImageViewer *preview;
    Table *table;
    QSplitter *splitter;

    bool brushActive = false;

    void init();
    void enableEditionTools(bool checked = true);
    void errorMsgBox(QString msg);

    void updateImageView();
    void switchToEdit(bool active);
    void matchButtonsStyle();
};

#endif // MAINWINDOW_H

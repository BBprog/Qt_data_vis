#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QMainWindow>
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
    void on_actionClose_triggered();

    void on_actionLoad_triggered();

    void on_actionFill_auto_triggered();

    void on_actionReinitialize_triggered();

    void on_actionFill_selected_triggered();

    void on_actionGenerate_Image_triggered();

    void on_actionPick_color_toggled(bool active);

    void on_actionShow_image_preview_triggered(bool checked);

    void on_actionFill_clicked_triggered(bool checked);

    void on_table_itemClicked(QTableWidgetItem *item);

private:
    const QColor Kblank = Qt::white;
    QColorDialog *colorPicker;
    bool brushActive = false;

    void enableEditionTools(bool active = true);
    void errorMsgBox(QString msg);

    void fillItem(int col, QString text, QColor color);
    void initTable(int nbColumn, QStringList headerLabels);
    bool findEmptyItem();
    void updateImageView();
};

#endif // MAINWINDOW_H

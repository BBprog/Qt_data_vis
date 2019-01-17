#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionClose_triggered();

    void on_actionLoad_triggered();

    void on_actionFill_auto_triggered();

    void on_actionPick_color_triggered();

    void on_actionReinitialize_triggered();

    void on_actionFill_selected_triggered();

    void on_actionGenerate_Image_triggered();

private:
    Ui::MainWindow *ui;
    QList<QMap<QString, QColor> > assignedColors;
    QColor currentColor;

    void enableEditionTools(bool active = true);
    void fillItem(int col, QString text, QColor color, int begin = 0);
    void initTable(int nbColumn, QStringList headerLabels);
    void errorMsgBox(QString msg);
};

#endif // MAINWINDOW_H

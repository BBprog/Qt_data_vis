#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>
#include <QScrollArea>

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    void loadImage(QImage image);

private:
    QLabel *imageLabel;
    QScrollArea *scrollArea;

};


#endif // IMAGEVIEWER_H

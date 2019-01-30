#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>
#include <QScrollArea>

class ImageViewer : public QLabel
{
    Q_OBJECT

public:
    ImageViewer(QWidget *parent = nullptr);
    void loadImage(QImage image);

};


#endif // IMAGEVIEWER_H

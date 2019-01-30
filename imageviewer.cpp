#include "imageviewer.h"

#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent)
    : QLabel(parent)
{ }

void ImageViewer::loadImage(QImage image)
{
    setPixmap(QPixmap::fromImage(image));
    adjustSize();
}

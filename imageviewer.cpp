#include "imageviewer.h"

#include <QDebug>

ImageViewer::ImageViewer(int sc, QWidget *parent)
    : QLabel(parent)
    , scale(sc)
{ }

void ImageViewer::loadImage(QImage image)
{
    int w = image.width();
    int h = image.height();
    (w < h) ? w = 1 : h = 1;

    p = QPixmap::fromImage(image);
    setPixmap(p.scaled(w*scale, h*scale, Qt::KeepAspectRatio));
    adjustSize();
}

void ImageViewer::setScale(int sc) { scale = sc; }
int ImageViewer::getScale() { return scale; }

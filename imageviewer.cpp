#include "imageviewer.h"

#include <QDebug>
#include <QStackedLayout>

ImageViewer::ImageViewer(QWidget *parent)
    : imageLabel(new QLabel)
    , scrollArea(new QScrollArea)
{
    setParent(parent);

    scrollArea->setWidget(imageLabel);
    QStackedLayout *stackedLayout = new QStackedLayout(scrollArea);
    setLayout(stackedLayout);
    setMinimumSize(100, 100);
}

void ImageViewer::loadImage(QImage image)
{
    imageLabel->setPixmap(QPixmap::fromImage(image));
    adjustSize();
}

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>

class ImageViewer : public QLabel
{
    Q_OBJECT

public:
    ImageViewer(int sc = 1, QWidget *parent = nullptr);
    void loadImage(QImage image);

    void setScale(int sc);
    int getScale();
private:
    int scale;
    QPixmap p;
};


#endif // IMAGEVIEWER_H

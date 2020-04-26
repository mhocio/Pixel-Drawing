#include "imagewindow.h"

ImageWindow::ImageWindow(QWidget *parent) : QWidget(parent)
{
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    mode = NONE;
}

void ImageWindow::_resize() {
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    qDebug() << "size:" << image.sizeInBytes();
}

bool operator==(const PixelWithColor& lhs, const PixelWithColor& rhs)
{
    return lhs.x== rhs.x && lhs.y == rhs.y && lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B;
}

bool operator!=(const PixelWithColor& lhs, const PixelWithColor& rhs)
{
    return !(lhs == rhs);
}

void ImageWindow::paintEvent(QPaintEvent*) {
    QTime time;
    time.start();

    QPainter painter(this);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    while (bits < bitsEnd) {
        *(bits) = 255;
        bits++;
    }

    /*
    for (IShape shape: shapes) {
        PixelWithColor pix = PixelWithColor(0, 0, 0, 0, 0);
        while ((pix = shape.generator()) != PixelWithColor(-1, -1, -1, -1, -1))
            setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
    }*/

    for (const auto &shape : shapes) {
        std::vector<PixelWithColor> pixels = shape->getPixels();
        for (PixelWithColor pix: pixels)
            setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
    }

    setPixel(100,380,0,0,0);

    painter.drawImage(0, 0, image);

    qDebug() << "drawImage time:" << time.elapsed();
}

bool ImageWindow::setPixel(int x, int y, int R, int G, int B) {
    uchar* bits = image.bits();

    if (x >= image.width() || y >= image.height())
        return false;

    *(bits + 3*x + 3*y*image.height()) = R;
    *(bits + 3*x + 3*y*image.height() + 1) = G;
    *(bits + 3*x + 3*y*image.height() + 2) = B;

    return true;
}

void ImageWindow::mouseDoubleClickEvent(QMouseEvent * mouseEvent) {
    qDebug() << Q_FUNC_INFO << mouseEvent->pos();
}

void ImageWindow::mouseMoveEvent(QMouseEvent * mouseEvent) {
    qDebug() << Q_FUNC_INFO << mouseEvent->pos();
}

void ImageWindow::mousePressEvent(QMouseEvent * mouseEvent) {
    qDebug() << Q_FUNC_INFO << mouseEvent->pos();

    switch (mode) {
    case DRAW:
        tmpLineX1 = mouseEvent->pos().x();
        tmpLineY1 = mouseEvent->pos().y();
        break;
    case NONE:
        break;
    }
}

void ImageWindow::mouseReleaseEvent(QMouseEvent * mouseEvent) {
    qDebug() << Q_FUNC_INFO << mouseEvent->pos();

    switch (mode) {
    case DRAW:
        if (pos().x() < width() && pos().y() < height()) {
            int X2 = mouseEvent->pos().x();
            int Y2 = mouseEvent->pos().y();

            auto line = std::make_unique<MyLine>(tmpLineX1, tmpLineY1, X2, Y2);

            if((abs(Y2 - tmpLineY1) > abs(X2 - tmpLineY1)))
                line->swapAB();

            shapes.push_back(std::move(line));
            mode = NONE;
            update();
        }
        break;
    case NONE:
        break;
    }
}

void ImageWindow::deleteAllShapes() {
    shapes.clear();
    update();
}

void ImageWindow::setModeDraw() {
    mode = DRAW;
}

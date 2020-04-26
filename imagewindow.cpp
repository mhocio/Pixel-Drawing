#include "imagewindow.h"

/*
ImageWindow::ImageWindow(MainWindow *mw) : mainWindow(mw) {
    mainWindow = mw;
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    mode = NONE;
}*/


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

    if (tmpPolygon != nullptr) {
        std::vector<PixelWithColor> pixels = tmpPolygon->getPixels();
        for (PixelWithColor pix: pixels)
            setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
    }

    setPixel(100, 380, 0, 0, 0);

    painter.drawImage(0, 0, image);

    //qDebug() << "drawImage time:" << time.elapsed();
}

bool ImageWindow::setPixel(int x, int y, int R, int G, int B) {
    uchar* bits = image.bits();

    if (x >= image.width() || y >= image.height()
            || x < 0 || y < 0)
        return false;

    *(bits + 3*x + 3*y*image.height()) = B;
    *(bits + 3*x + 3*y*image.height() + 1) = G;
    *(bits + 3*x + 3*y*image.height() + 2) = R;

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
    case LINE:
        tmpX1 = mouseEvent->pos().x();
        tmpY1 = mouseEvent->pos().y();
        break;
    case CIRCLE:
        tmpX1 = mouseEvent->pos().x();
        tmpY1 = mouseEvent->pos().y();
        break;
    case NONE:
        break;
    }
}

float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

void ImageWindow::mouseReleaseEvent(QMouseEvent * mouseEvent) {
    qDebug() << Q_FUNC_INFO << mouseEvent->pos();

    int X2 = mouseEvent->pos().x();
    int Y2 = mouseEvent->pos().y();

    bool needToUpdate = false;

    switch (mode) {
    case LINE:
        if (pos().x() < width() && pos().y() < height()
            && pos().x() >= 0 && pos().y() >= 0) {

            auto line = std::make_unique<MyLine>(tmpX1, tmpY1, X2, Y2);

            shapes.push_back(std::move(line));
            needToUpdate = true;
        }
        break;
    case CIRCLE:
        if (pos().x() < width() && pos().y() < height()
            && pos().x() >= 0 && pos().y() >= 0) {

            int radius = (int)distance(tmpX1, tmpY1, X2, Y2);
            qDebug() << "radius: " << radius << " X: " << tmpX1 << " Y: " << tmpY1;

            auto circle = std::make_unique<MyCircle>(tmpX1, tmpY1, radius);

            shapes.push_back(std::move(circle));
            needToUpdate = true;
        }
        break;
    case POLYGON:
        qDebug() << Q_FUNC_INFO << "MOUSE POLYGON: " << newShape;

        if (pos().x() < width() && pos().y() < height()
            && pos().x() >= 0 && pos().y() >= 0) {

            if (newShape) {
                tmpPolygon = std::make_unique<myPolygon>();
                tmpPolygon->addPoint(X2, Y2);
            } else
                tmpPolygon->addPoint(X2, Y2);

            needToUpdate = true;
        }
        break;
    case NONE:
        break;
    }

    if (needToUpdate) {
        //mode = NONE;
        update();
        displayShapesList();
        newShape = false;
    }
}

void ImageWindow::endDrawingPolygon() {
    if (tmpPolygon != nullptr) {
        if (tmpPolygon->points.size() > 2) {
            MyLine line = MyLine(tmpPolygon->points[tmpPolygon->getPointsSize()-1].first,
                    tmpPolygon->points[tmpPolygon->getPointsSize()-1].second,
                    tmpPolygon->points[0].first,
                    tmpPolygon->points[0].second );
            tmpPolygon->lines.push_back(line);

            shapes.push_back(std::move(tmpPolygon));
        }
        tmpPolygon = nullptr;
        update();
        displayShapesList();
        newShape = false;
    }
}

void ImageWindow::addPolygon() {
    if (tmpPolygon != nullptr) {
        shapes.push_back(std::move(tmpPolygon));
        tmpPolygon = nullptr;

        update();
        displayShapesList();
        newShape = false;
    }
}

void ImageWindow::displayShapesList() {
    QWindowList windows = QGuiApplication::topLevelWindows();
    getAllShapesStrings();
    for (QWindow *window: windows) {
        qDebug() << Q_FUNC_INFO << window->title();

        /*if (window->title() == "MainWindow") {
            auto mainWin = qobject_cast<MainWindow*>(window);
            //mainWin
        }*/
    }
}

std::vector<std::string> ImageWindow::getAllShapesStrings() {
    std::vector <std::string> vec;

    for (const auto &shape : this->shapes) {
        //qDebug() << "shape: " << shape->ToString().c_str();
        vec.push_back(shape->ToString());
    }

    return vec;
}

void ImageWindow::updateShapeColor(QListWidgetItem* item, QColor color) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            shape->setColor(color);
            update();
            break;
        }
    }
}

void ImageWindow::deleteShape(QListWidgetItem* item) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
            update();
            break;
        }
    }
}

void ImageWindow::deleteAllShapes() {
    shapes.clear();
    update();
}

void ImageWindow::setModeDrawLine() {
    mode = LINE;
}

void ImageWindow::setModeDrawCircle() {
    mode = CIRCLE;
}

void ImageWindow::setModeDrawPolygon() {
    mode = POLYGON;
}

void ImageWindow::setModeDrawNone() {
    mode = NONE;
}

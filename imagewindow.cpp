#include "imagewindow.h"

/*
ImageWindow::ImageWindow(MainWindow *mw) : mainWindow(mw) {
    mainWindow = mw;
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    mode = NONE;
}*/

float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

ImageWindow::ImageWindow(QWidget *parent) : QWidget(parent)
{
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    mode = NONE;
    antiAliased_mode = false;
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

void ImageWindow::TurnOnOffAntiAliasing() {
    antiAliased_mode = !antiAliased_mode;
    update();
}

void ImageWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    while (bits < bitsEnd) {
        *(bits) = 255;
        bits++;
    }

    for (const auto &shape : shapes) {
        // filling polygons
        myPolygon *poly = dynamic_cast<myPolygon*>(shape.get());
        if (poly != nullptr && poly->isFilled) {
            for (PixelWithColor pix: poly->getFillingPixels())
                setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
            continue;
        }

        std::vector<PixelWithColor> pixels;
        if (!antiAliased_mode)
            pixels = shape->getPixels();
        else
            pixels = shape->getPixelsAliased();

        for (PixelWithColor pix: pixels)
            setPixel(pix.x, pix.y,
                     255 - (255-pix.R)*pix.intensity,
                     255 - (255-pix.G)*pix.intensity,
                     255 - (255-pix.B)*pix.intensity);
    }

    if (tmpPolygon != nullptr) {
        std::vector<PixelWithColor> pixels = tmpPolygon->getPixels();
        for (PixelWithColor pix: pixels)
            setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
    }

    painter.drawImage(0, 0, image);
}

bool ImageWindow::setPixel(int x, int y, int R, int G, int B) {
    uchar* bits = image.bits();

    if (x >= image.width() || y >= image.height()
            || x < 0 || y < 0)
        return false;

    *(bits + 3*x + 3*y*image.height()) = B;
    *(bits + 3*x + 3*y*image.height() + 1) = G;
    *(bits + 3*x + 3*y*image.height() + 2) = R;

    // qDebug() << R << " " << G << " " << B;

    return true;
}

void ImageWindow::mouseDoubleClickEvent(QMouseEvent * mouseEvent) {
    // qDebug() << Q_FUNC_INFO << mouseEvent->pos();
}

void ImageWindow::mouseMoveEvent(QMouseEvent * mouseEvent) {
    // qDebug() << Q_FUNC_INFO << mouseEvent->pos();

    int X2 = mouseEvent->pos().x();
    int Y2 = mouseEvent->pos().y();

    bool needToUpdate = false;

    switch(mode) {
    case EDIT_LINE:
    {
        if (minimalDistance < MOUSE_RADIUS) {
            if (firstPoint) {
                closestLine->x1 = X2;
                closestLine->y1 = Y2;
            } else {
                closestLine->x2 = X2;
                closestLine->y2 = Y2;
            }
            needToUpdate = true;
        }
    }
        break;
    case MOVE_CIRCLE:
    {
        if (minimalDistance < MOUSE_RADIUS) {
            closestCircle->X = X2;
            closestCircle->Y = Y2;
            needToUpdate = true;
        }
    }
        break;
    case RESIZE_CIRCLE: {
        if (minimalDistance < MOUSE_RADIUS) {
            closestCircle->radius = distance(closestCircle->X, closestCircle->Y,
                                        X2, Y2);
            needToUpdate = true;
        }
    }
        break;
    case MOVE_POLYGON:
    {
        if (minimalDistance < MOUSE_RADIUS) {
            std::pair<int, int> centre = closestPolygon->compute2DCentroid();
            int dx = X2 - centre.first;
            int dy = Y2 - centre.second;

            closestPolygon->move(dx, dy);
            needToUpdate = true;
        }
    }
        break;
    case NONE:
        break;
    }

    if (needToUpdate) {
        //mode = NONE;
        update();
        newShape = false;
    }
}

void ImageWindow::mousePressEvent(QMouseEvent * mouseEvent) {
    // qDebug() << Q_FUNC_INFO << mouseEvent->pos();
    tmpX1 = mouseEvent->pos().x();
    tmpY1 = mouseEvent->pos().y();

    switch (mode) {
    case EDIT_LINE:
    {
        minimalDistance = 1000000;

        for (auto &shape: shapes) {
            if (typeid(*shape).name() == typeid(MyLine).name()) {

                MyLine line = dynamic_cast<MyLine&>(*shape);

                if (distance(tmpX1, tmpY1, line.x1, line.y1) < minimalDistance) {
                    minimalDistance = distance(tmpX1, tmpY1, line.x1, line.y1);
                    firstPoint = true;
                    closestLine = dynamic_cast<MyLine*>(shape.get());
                }

                if (distance(tmpX1, tmpY1, line.x2, line.y2) < minimalDistance) {
                    minimalDistance = distance(tmpX1, tmpY1, line.x2, line.y2);
                    firstPoint = false;
                    closestLine = dynamic_cast<MyLine*>(shape.get());
                }
            }
        }
    }
        break;
    case MOVE_CIRCLE:
    {
        minimalDistance = 1000000;
        for (auto &shape: shapes) {
            if (typeid(*shape).name() == typeid(MyCircle).name()) {

                MyCircle circle = dynamic_cast<MyCircle&>(*shape);

                if (distance(tmpX1, tmpY1, circle.X, circle.Y) < minimalDistance) {
                    minimalDistance = distance(tmpX1, tmpY1, circle.X, circle.Y);
                    closestCircle = dynamic_cast<MyCircle*>(shape.get());
                }

            }
        }
    }
        break;
    case RESIZE_CIRCLE:
    {
        minimalDistance = 1000000;
        for (auto &shape: shapes) {
            if (typeid(*shape).name() == typeid(MyCircle).name()) {

                MyCircle circle = dynamic_cast<MyCircle&>(*shape);

                unsigned int d = std::abs(distance(tmpX1, tmpY1, circle.X, circle.Y) - circle.radius);
                if (d < minimalDistance) {
                    minimalDistance = d;
                    closestCircle = dynamic_cast<MyCircle*>(shape.get());
                }

            }
        }
    }
        break;
    case MOVE_POLYGON:
    {
        minimalDistance = 1000000;
        for (auto &shape: shapes) {
            myPolygon polygon;
            try {
                polygon = dynamic_cast<myPolygon&>(*shape);
            } catch (std::exception e) {
                continue;
            }

            std::pair<int, int> polygonCentre = polygon.compute2DCentroid();
            unsigned int d = distance(tmpX1, tmpY1, polygonCentre.first, polygonCentre.second);

            if (d < minimalDistance) {
                minimalDistance = d;
                closestPolygon = dynamic_cast<myPolygon*>(shape.get());
            }
        }
    }
        break;
    case PIZZA:
    {
        if (tmpPizza == nullptr)
            tmpPizza = std::make_unique<myPizza>();

        if (pizzaPointNumber == 0) {
            tmpPizza->pA.first = tmpX1;
            tmpPizza->pA.second = tmpY1;
            pizzaPointNumber++;
        } else if (pizzaPointNumber == 1) {
            tmpPizza->pB.first = tmpX1;
            tmpPizza->pB.second = tmpY1;
            pizzaPointNumber++;
        } else if (pizzaPointNumber == 2) {
            tmpPizza->pC.first = tmpX1;
            tmpPizza->pC.second = tmpY1;
            pizzaPointNumber = 0;

            shapes.push_back(std::move(tmpPizza));
            update();
            tmpPizza = nullptr;

            qDebug() << Q_FUNC_INFO << "ADD PIZZA";
        }
    }
        break;
    case NONE:
        break;
    }
}

void ImageWindow::mouseReleaseEvent(QMouseEvent * mouseEvent) {
    // qDebug() << Q_FUNC_INFO << mouseEvent->pos();

    int X2 = mouseEvent->pos().x();
    int Y2 = mouseEvent->pos().y();

    bool needToUpdate = false;

    switch (mode) {
    case LINE:
        if (pos().x() < width() && pos().y() < height()
            && pos().x() >= 0 && pos().y() >= 0) {

            auto line = std::make_unique<MyLine>(tmpX1, tmpY1, X2, Y2);
            line->setThickness(brushThickness);

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
                tmpPolygon->setThickness(brushThickness);
                tmpPolygon->addPoint(X2, Y2);
            } else
                tmpPolygon->addPoint(X2, Y2);

            needToUpdate = true;
        }
        break;
    case RECTANGLE:
    {
        if (pos().x() < width() && pos().y() < height()
            && pos().x() >= 0 && pos().y() >= 0) {

            std::pair<int, int> A = std::make_pair(tmpX1, tmpY1);
            std::pair<int, int> B = std::make_pair(X2, Y2);
            auto rectangle = std::make_unique<myRectangle>(A, B);
            rectangle->setThickness(brushThickness);

            shapes.push_back(std::move(rectangle));
            needToUpdate = true;
        }
    }
        break;
    case EDIT_LINE:
    {
        if (minimalDistance < MOUSE_RADIUS) {
            if (firstPoint) {
                closestLine->x1 = X2;
                closestLine->y1 = Y2;
            } else {
                closestLine->x2 = X2;
                closestLine->y2 = Y2;
            }
            needToUpdate = true;
        }
    }
        break;
    case NONE:
        break;
    }

    if (needToUpdate) {
        //mode = NONE;
        update();
        newShape = false;
    }
}

void ImageWindow::endDrawingPolygon() {
    if (tmpPolygon != nullptr) {
        if (tmpPolygon->points.size() > 2) {

            tmpPolygon->setFinished();
            tmpPolygon->setThickness(brushThickness);
            shapes.push_back(std::move(tmpPolygon));
        }
        tmpPolygon = nullptr;
        update();
        newShape = false;
        mode = NONE;
    }
}

void ImageWindow::addPolygon() {
    if (tmpPolygon != nullptr) {
        tmpPolygon->setThickness(brushThickness);
        shapes.push_back(std::move(tmpPolygon));
        tmpPolygon = nullptr;
        update();
        newShape = false;
        mode = NONE;
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

void ImageWindow::fillPolygon(QListWidgetItem* item, QColor color) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            myPolygon *poly = dynamic_cast<myPolygon*>(shape.get());
            if (poly == nullptr) {
                qDebug() << "not polygon";
                return;
            }

            poly->setFilled(color);
            update();
        }
    }
}

void ImageWindow::fillPolygon(QListWidgetItem* item, QImage image) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            myPolygon *poly = dynamic_cast<myPolygon*>(shape.get());
            if (poly == nullptr) {
                qDebug() << "not polygon";
                return;
            }

            poly->setFilled(image);
            update();
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

void ImageWindow::setShapeThickness(QListWidgetItem*item, int thickness) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            shape->setThickness(thickness);
            update();
            break;
        }
    }
}

void ImageWindow::createNewPolygon(QListWidgetItem* clipBoundaryPolygon, QListWidgetItem* polygonToClip) {
    myPolygon* clipPolygon = nullptr;
    myPolygon* boundaryPolygon = nullptr;

    if (clipBoundaryPolygon == nullptr
            ||polygonToClip == nullptr)
        return;

    for (const auto &shape : shapes) {
        if (shape->ToString() == clipBoundaryPolygon->text().toStdString())
            boundaryPolygon = dynamic_cast<myPolygon*>(shape.get());
        else if (shape->ToString() == polygonToClip->text().toStdString())
            clipPolygon = dynamic_cast<myPolygon*>(shape.get());
    }

    if (!clipPolygon->isConvex()) {
        QMessageBox msgBox;
        msgBox.setText("The clipping polygon is not convex!");
        msgBox.exec();
        return;
    }

    if (clipPolygon != nullptr && boundaryPolygon != nullptr) {
        SutherlandHodgman S = SutherlandHodgman();
        std::vector<QPoint> points = S.GetIntersectedPoints(*clipPolygon, *boundaryPolygon);

        qDebug() << "POINTS: ";
        for (auto p: points)
            qDebug() << p.x() << " " << p.y();

        auto newPoly = std::make_unique<myPolygon>();
        newPoly->thickness = 2;
        for (QPoint point: points)
            newPoly->addPoint(point.x(), point.y());
        newPoly->setFinished();

        shapes.push_back(std::move(newPoly));
        update();
    }
}


void ImageWindow::updateBrushThickness(int t) {
    brushThickness = t;
}

QString ImageWindow::getMode() {
    switch (mode)
    {
        case LINE: return "Drawing line";
        case CIRCLE: return "Drawing cyrcle";
        case POLYGON: return "Drawing polygon";
        case NONE: return "";
        case EDIT_LINE: return "Editing line";
        case MOVE_CIRCLE: return "Moving circle";
        case RESIZE_CIRCLE: return "Resizing circle";
        case MOVE_POLYGON: return "Moving Polygon";
        case PIZZA: return "Drawing pizza";
        case RECTANGLE: return "Drawing rectangle";
    default: return "";
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

void ImageWindow::setModeDrawRectangle() {
    mode = RECTANGLE;
}

void ImageWindow::setModeDrawNone() {
    mode = NONE;
}

void ImageWindow::setModeEditLine() {
    mode = EDIT_LINE;
}

void ImageWindow::setModeMoveCircle() {
    mode = MOVE_CIRCLE;
}

void ImageWindow::setModeResizeCircle() {
    mode = RESIZE_CIRCLE;
}

void ImageWindow::setModeMovePolygon() {
    mode = MOVE_POLYGON;
}

void ImageWindow::setModeDrawPizza() {
    mode = PIZZA;
    pizzaPointNumber = 0;
}

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

void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

ImageWindow::ImageWindow(QWidget *parent) : QWidget(parent)
{
    image = QImage(width(), height(), QImage::Format_BGR888);  // check if null
    mode = NONE;
    antiAliased_mode = false;

    meshCube.tri = {
        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    };

    // Projection Matrix
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)_height / (float)_width;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

    qDebug() << "size:::";
    qDebug() << width();
    qDebug() << height();

    repaint();

    //run();
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
    repaint();
}

void ImageWindow::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    auto line1 = std::make_unique<MyLine>(x1, y1, x2, y2);
    auto line2 = std::make_unique<MyLine>(x2, y2, x3, y3);
    auto line3 = std::make_unique<MyLine>(x3, y3, x1, y1);
    //line->setThickness(brushThickness);

    shapes.push_back(std::move(line1));
    shapes.push_back(std::move(line2));
    shapes.push_back(std::move(line3));
}

void ImageWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    uchar* bits = image.bits();
    uchar* bitsEnd = bits + image.sizeInBytes();

    while (bits < bitsEnd) {
        *(bits) = 255;
        bits++;
    }

    // Set up rotation matrices
    mat4x4 matRotZ, matRotX;
    fTheta += 1.0f * fElapsedTime;

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fTheta * 0.5f);
    matRotX.m[1][2] = sinf(fTheta * 0.5f);
    matRotX.m[2][1] = -sinf(fTheta * 0.5f);
    matRotX.m[2][2] = cosf(fTheta * 0.5f);
    matRotX.m[3][3] = 1;

    for (auto t : meshCube.tri) {
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        // Rotate in Z-Axis
        MultiplyMatrixVector(t.p[0], triRotatedZ.p[0], matRotZ);
        MultiplyMatrixVector(t.p[1], triRotatedZ.p[1], matRotZ);
        MultiplyMatrixVector(t.p[2], triRotatedZ.p[2], matRotZ);

        // Rotate in X-Axis
        MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

        // Offset into the screen
        triTranslated = t;
        triTranslated.p[0].z = t.p[0].z + 3.0f;
        triTranslated.p[1].z = t.p[1].z + 3.0f;
        triTranslated.p[2].z = t.p[2].z + 3.0f;

        MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
        MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
        MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

        // Scale into view
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
        triProjected.p[0].x *= 0.5f * (float)_width;
        triProjected.p[0].y *= 0.5f * (float)_height;
        triProjected.p[1].x *= 0.5f * (float)_width;
        triProjected.p[1].y *= 0.5f * (float)_height;
        triProjected.p[2].x *= 0.5f * (float)_width;
        triProjected.p[2].y *= 0.5f * (float)_height;

        drawTriangle(triProjected.p[0].x, triProjected.p[0].y,
                        triProjected.p[1].x, triProjected.p[1].y,
                        triProjected.p[2].x, triProjected.p[2].y);
    }

     for (const auto &shape : shapes) {
         // filling polygons
//         myPolygon *poly = dynamic_cast<myPolygon*>(shape.get());
//         if (poly != nullptr && poly->isFilled) {
//             for (PixelWithColor pix: poly->getFillingPixels())
//                 setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
//             continue;
//         }

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

    // if (tmpPolygon != nullptr) {
    //     std::vector<PixelWithColor> pixels = tmpPolygon->getPixels();
    //     for (PixelWithColor pix: pixels)
    //         setPixel(pix.x, pix.y, pix.R, pix.G, pix.B);
    // }

    painter.drawImage(0, 0, image);

    painter.end();
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
    case MOVE_RECTANGLE_VERTEX:
    {
        polygonToEdit->moveVertex(X2, Y2);
        needToUpdate = true;
    }
        break;
    case NONE:
        break;
    }

    if (needToUpdate) {
        //mode = NONE;
        repaint();
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
            repaint();
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
        repaint();
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
        repaint();
        newShape = false;
        mode = NONE;
    }
}

void ImageWindow::addPolygon() {
    if (tmpPolygon != nullptr) {
        tmpPolygon->setThickness(brushThickness);
        shapes.push_back(std::move(tmpPolygon));
        tmpPolygon = nullptr;
        repaint();
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
            repaint();
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
            repaint();
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
            repaint();
        }
    }
}

void ImageWindow::deleteShape(QListWidgetItem* item) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
            repaint();
            break;
        }
    }
}

void ImageWindow::setShapeThickness(QListWidgetItem*item, int thickness) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            shape->setThickness(thickness);
            repaint();
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
        repaint();
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
        case MOVE_RECTANGLE_EDGE: return "Moving Rectangle Edge";
        case MOVE_RECTANGLE_VERTEX: return "Moving Rectangle Vertex";
    default: return "";
    }
}

void ImageWindow::deleteAllShapes() {
    shapes.clear();
    repaint();
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

void ImageWindow::setModeMovePolygonEdge() {
    mode = MOVE_RECTANGLE_EDGE;
}

void ImageWindow::setModeMovePolygonVertex(QListWidgetItem* item) {
    for (const auto &shape : shapes) {
        if (shape->ToString() == item->text().toStdString()) {
            polygonToEdit = dynamic_cast<myPolygon*>(shape.get());
            mode = MOVE_RECTANGLE_VERTEX;
        }
    }
}

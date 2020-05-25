#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QWidget>
#include <QWindow>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QGuiApplication>
#include <QMessageBox>

#include <cmath>
#include <string.h>

#include "myline.h"
#include "mycircle.h"
#include "mypolygon.h"
#include "mypizza.h"
#include "sutherlandhodgman.h"
#include <myrectangle.h>

//class MainWindow;

class ImageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = nullptr);
    //ImageWindow(MainWindow *mw);

    void paintEvent(QPaintEvent*);
    void _resize();
    bool setPixel(int x, int y, int R, int G, int B);

    void deleteAllShapes();
    std::vector <std::string> getAllShapesStrings();

    void updateShapeColor(QListWidgetItem* item, QColor color);
    void fillPolygon(QListWidgetItem* item, QColor color);
    void deleteShape(QListWidgetItem* item);
    void setShapeThickness(QListWidgetItem*item, int thickness);

    void createNewPolygon(QListWidgetItem* clipBoundaryPolygon, QListWidgetItem* polygonToClip);

    void updateBrushThickness(int t);
    unsigned int brushThickness = 1;

    void endDrawingPolygon();

    virtual void mouseDoubleClickEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    void setModeDrawLine();
    void setModeDrawCircle();
    void setModeDrawPolygon();
    void setModeDrawNone();
    void setModeDrawRectangle();

    void setModeEditLine();
    void setModeMoveCircle();
    void setModeResizeCircle();
    void setModeMovePolygon();

    void setModeDrawPizza();

    QString getMode();

    void createNewShape() {
        newShape = true;
        addPolygon();
    }

    void addPolygon();

    const unsigned int MOUSE_RADIUS = 100;

    void TurnOnOffAntiAliasing();

private:
    QImage image;
    enum drawMode
    {
        LINE,
        CIRCLE,
        POLYGON,
        NONE,
        EDIT_LINE,
        MOVE_CIRCLE,
        RESIZE_CIRCLE,
        MOVE_POLYGON,
        PIZZA,
        RECTANGLE
    };
    drawMode mode;

    bool antiAliased_mode;

    int tmpX1;
    int tmpY1;

    std::vector<std::unique_ptr<IShape>> shapes;
    bool newShape = true;
    std::unique_ptr<myPolygon> tmpPolygon = nullptr;

    // used to EDIT_LINE
    unsigned int minimalDistance;
    MyLine *closestLine;
    bool firstPoint;

    // used to EDIT_CIRCLE
    MyCircle *closestCircle;

    // used to MOVE_POLYGON
    myPolygon *closestPolygon;

    // used to PIZZA
    std::unique_ptr<myPizza> tmpPizza = nullptr;
    int pizzaPointNumber = 0;


signals:

};

#endif // IMAGEWINDOW_H

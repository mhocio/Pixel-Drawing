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
#include <QPixmap>
#include <QElapsedTimer>
#include <QTimer>
//#include "HelloThread.h"

#include <cmath>
#include <string.h>
#include <thread>

#include "myline.h"
#include "mycircle.h"
#include "mypolygon.h"
#include "mypizza.h"
#include "sutherlandhodgman.h"
#include <myrectangle.h>

//class MainWindow

struct vec3d {
    float x, y, z;
};

struct triangle {
    vec3d p[3];
};

struct mesh {
    std::vector<triangle> tri;
};

struct mat4x4 {
    float m[4][4] = {0};
};

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
    void fillPolygon(QListWidgetItem* item, QImage image);
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

    void setModeMovePolygonEdge();
    void setModeMovePolygonVertex(QListWidgetItem* item);

    void setModeDrawPizza();

    QString getMode();

    void createNewShape() {
        newShape = true;
        addPolygon();
    }

    void addPolygon();
    std::vector<std::unique_ptr<IShape>> shapes;

    const unsigned int MOUSE_RADIUS = 100;

    void TurnOnOffAntiAliasing();

    ////////

//    void run() {
//        while(true) {
//            //QTimer *timer = new QTimer(this);
//            //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//            //timer->start(16);
//            fElapsedTime++;
//            repaint();
//            //wait(1);
//        }
//    }
    void _repaint() {
        repaint();
    }
    float fElapsedTime = 0;

private:
    mesh meshCube;
    mat4x4 matProj;
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    int _width = 700;
    int _height = 700;
    float fTheta = 0;

    //void foo(int Z);

    //////
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
        RECTANGLE,
        MOVE_RECTANGLE_EDGE,
        MOVE_RECTANGLE_VERTEX
    };
    drawMode mode;

    bool antiAliased_mode;

    int tmpX1;
    int tmpY1;

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
    myPolygon *polygonToEdit;

    // used to PIZZA
    std::unique_ptr<myPizza> tmpPizza = nullptr;
    int pizzaPointNumber = 0;


signals:

};

#endif // IMAGEWINDOW_H

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

#include <cmath>
#include <string.h>

#include "myline.h"
#include "mycircle.h"
#include "mypolygon.h"

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
    void displayShapesList();
    std::vector <std::string> getAllShapesStrings();

    void updateShapeColor(QListWidgetItem* item, QColor color);
    void deleteShape(QListWidgetItem* item);

    void endDrawingPolygon();

    virtual void mouseDoubleClickEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    void setModeDrawLine();
    void setModeDrawCircle();
    void setModeDrawPolygon();
    void setModeDrawNone();

    void setModeEditLine();

    void createNewShape() {
        newShape = true;
        addPolygon();
    }

    void addPolygon();

private:
    QImage image;
    enum drawMode
    {
        LINE,
        CIRCLE,
        POLYGON,
        NONE,
        EDIT_LINE
    };
    drawMode mode;

    MyLine tmpLine;
    int tmpX1;
    int tmpY1;

    std::vector<std::unique_ptr<IShape>> shapes;
    bool newShape = true;
    std::unique_ptr<myPolygon> tmpPolygon = nullptr;

    const unsigned int MOUSE_RADIUS = 10;

    //QPainter painter;

signals:

};

#endif // IMAGEWINDOW_H

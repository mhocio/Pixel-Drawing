#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QWidget>
#include <QWindow>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMouseEvent>
#include <cmath>
#include <QGuiApplication>
#include "myline.h"
#include "mycircle.h"

class MainWindow;

class ImageWindow : public QWidget
{
    Q_OBJECT
public:
    //explicit ImageWindow(QWidget *parent = nullptr);
    ImageWindow(MainWindow *mw);

    MainWindow *getMainWindow() {
        return mainWindow;
    }

    MainWindow *mainWindow;

    void paintEvent(QPaintEvent*);
    void _resize();
    bool setPixel(int x, int y, int R, int G, int B);

    void deleteAllShapes();
    void displayShapesList();

    virtual void mouseDoubleClickEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    void setModeDrawLine();
    void setModeDrawCircle();
    void setModeDrawPolygon();
    void setModeDrawNone();

private:
    QImage image;
    enum drawMode
    {
        LINE,
        CIRCLE,
        POLYGON,
        NONE
    };
    drawMode mode;

    MyLine tmpLine;
    int tmpX1;
    int tmpY1;

    std::vector<std::unique_ptr<IShape>> shapes;

    //QPainter painter;

signals:

};

#endif // IMAGEWINDOW_H

#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QMouseEvent>
#include "myline.h"

class ImageWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void _resize();
    bool setPixel(int x, int y, int R, int G, int B);

    void deleteAllShapes();

    virtual void mouseDoubleClickEvent(QMouseEvent * mouseEvent);
    virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
    virtual void mousePressEvent(QMouseEvent * mouseEvent);
    virtual void mouseReleaseEvent(QMouseEvent * mouseEvent);

    void setModeDraw();
    void setModeCircle();
    void setModeShape();
    void setModeNone();

private:
    QImage image;
    enum drawMode
    {
        DRAW,
        CIRCLE,
        POLYGON,
        NONE
    };
    drawMode mode;

    MyLine tmpLine;
    int tmpLineX1;
    int tmpLineY1;

    std::vector<std::unique_ptr<IShape>> shapes;

    //QPainter painter;

signals:

};

#endif // IMAGEWINDOW_H

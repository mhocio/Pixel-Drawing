#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QBrush>
#include <QFileDialog>
#include <QPixmap>
#include <fstream>
#include "imagewindow.h"
#include "nlohmann/json.hpp"
#include "mycircle.h"
#include "myline.h"
#include "mypizza.h"
#include "mypolygon.h"
#include "myrectangle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    QListWidgetItem* getListWidgetSelectedItem();

private slots:
    void on_drawLineButton_clicked();

    void on_deleteAllButton_clicked();

    void on_drawCircleButton_clicked();

    void on_listFiguresButton_clicked();

    void updateListWidget();

    void on_changeColorButton_clicked();

    void on_deleteFigureButton_clicked();

    void on_drawPolygonButton_clicked();

    void on_endDrawingPolygonButton_clicked();

    void on_editLineButton_clicked();

    void on_moveCircleButton_clicked();

    void on_resizeCircleButton_clicked();

    void updateStatusBar();

    void on_movePolygonButton_clicked();

    void on_setThicknessButton_clicked();

    void on_brushThickness_valueChanged(int arg1);

    void on_pizzaSliceButton_clicked();

    void on_checkBoxAntiAliasing_stateChanged(int arg1);

    void on_drawRectangleButton_clicked();

    void on_fillPolygonColorButton_clicked();

    void on_setBoundaryButton_clicked();

    void on_setClipButton_clicked();

    void on_clipPolygonButton_clicked();

    void on_fillPolygonwithImageButton_clicked();

    void on_moveEdgeButton_clicked();

    void on_movePolygonVertexButton_clicked();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    ImageWindow *sceneQWidget;

    QListWidgetItem *clipBoundaryPolygon = nullptr;
    QListWidgetItem *polygonToClip = nullptr;
};

#endif // MAINWINDOW_H

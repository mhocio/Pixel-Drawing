#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QBrush>
#include "imagewindow.h"

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

    void on_boundaryFillButton_clicked();

private:
    ImageWindow *sceneQWidget;
};

#endif // MAINWINDOW_H

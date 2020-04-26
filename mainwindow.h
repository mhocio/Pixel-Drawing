#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void on_drawLineButton_clicked();

    void on_deleteAllButton_clicked();

private:
    Ui::MainWindow *ui;
    ImageWindow *sceneQWidget;
};
#endif // MAINWINDOW_H

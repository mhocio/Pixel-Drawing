#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sceneQWidget = new ImageWindow(ui->paintArea);
    sceneQWidget->resize(ui->paintArea->width(), ui->paintArea->height());
    sceneQWidget->_resize();

    qDebug() << ui->paintArea->width() << " " << ui->paintArea->height();
    qDebug() << ui->verticalFramePaint->width() << " " << ui->verticalFramePaint->height();

    sceneQWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawLineButton_clicked()
{
    sceneQWidget->setModeDraw();
}

void MainWindow::on_deleteAllButton_clicked()
{
    sceneQWidget->deleteAllShapes();
}

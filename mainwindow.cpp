#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sceneQWidget = new ImageWindow(ui->paintArea);
    //sceneQWidget = new ImageWindow(this);

    sceneQWidget->resize(ui->paintArea->width(), ui->paintArea->height());
    //sceneQWidget->resize(400, 400);
    sceneQWidget->_resize();

    qDebug() << ui->paintArea->width() << " " << ui->paintArea->height();
    qDebug() << ui->verticalFramePaint->width() << " " << ui->verticalFramePaint->height();

    sceneQWidget->show();
}

void MainWindow::updateStatusBar() {
    ui->statusbar->showMessage(sceneQWidget->getMode());
    ui->statusbar->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawLineButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawLine();
    updateStatusBar();
}

void MainWindow::on_deleteAllButton_clicked()
{
    sceneQWidget->deleteAllShapes();

    updateListWidget();
}

void MainWindow::on_drawCircleButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawCircle();
    updateStatusBar();
}

void MainWindow::updateListWidget() {
    ui->listWidget->clear();

    std::vector<std::string> list = sceneQWidget->getAllShapesStrings();
    for (std::string item: list) {
        ui->listWidget->addItem(item.c_str());
    }

    ui->listWidget->update();
}

void MainWindow::on_listFiguresButton_clicked()
{
    qDebug() << Q_FUNC_INFO;
    updateListWidget();
}

QListWidgetItem* MainWindow::getListWidgetSelectedItem() {
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();


    if (list.count() != 1)
        return NULL;

    QListWidgetItem* item = list.first();
    item->setForeground(QColor(255, 0, 0)); // sets red text

    /* pItem->setForeground(Qt::red); // sets red text
    pItem->setBackground(Qt::green);  // sets green background */

    return item;
}

void MainWindow::on_changeColorButton_clicked()
{
    QListWidgetItem* item = getListWidgetSelectedItem();

    if (item == NULL)
        return;

    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if (color.isValid()) {
      sceneQWidget->updateShapeColor(item, color);
      item->setForeground(color);
    }
}

void MainWindow::on_deleteFigureButton_clicked()
{
    QListWidgetItem* item = getListWidgetSelectedItem();

    if (item == NULL)
        return;

    sceneQWidget->deleteShape(item);
    updateListWidget();
}

void MainWindow::on_drawPolygonButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawPolygon();
    updateStatusBar();
}

void MainWindow::on_endDrawingPolygonButton_clicked()
{
    sceneQWidget->endDrawingPolygon();
    updateStatusBar();
}

void MainWindow::on_editLineButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeEditLine();
    updateStatusBar();
}

void MainWindow::on_moveCircleButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeMoveCircle();
    updateStatusBar();
}

void MainWindow::on_resizeCircleButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeResizeCircle();
    updateStatusBar();
}

void MainWindow::on_movePolygonButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeMovePolygon();
    updateStatusBar();
}

void MainWindow::on_setThicknessButton_clicked()
{
    QListWidgetItem* item = getListWidgetSelectedItem();

    if (item == NULL)
        return;

    sceneQWidget->setShapeThickness(item, ui->spinBoxThickness->value());
}

void MainWindow::on_brushThickness_valueChanged(int arg1)
{
    sceneQWidget->updateBrushThickness(ui->brushThickness->value());
}

void MainWindow::on_pizzaSliceButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawPizza();
    updateStatusBar();
}

void MainWindow::on_checkBoxAntiAliasing_stateChanged(int arg1)
{
    sceneQWidget->TurnOnOffAntiAliasing();
}

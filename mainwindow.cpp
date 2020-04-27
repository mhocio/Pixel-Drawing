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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawLineButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawLine();
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

    if (color.isValid())
      sceneQWidget->updateShapeColor(item, color);
}

void MainWindow::on_deleteFigureButton_clicked()
{
    QListWidgetItem* item = getListWidgetSelectedItem();

    if (item == NULL)
        return;

    sceneQWidget->deleteShape(item);
}

void MainWindow::on_drawPolygonButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawPolygon();
}

void MainWindow::on_endDrawingPolygonButton_clicked()
{
    sceneQWidget->endDrawingPolygon();
}

void MainWindow::on_editLineButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeEditLine();
}

void MainWindow::on_moveCircleButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeMoveCircle();
}

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
    sceneQWidget->setModeDrawLine();
}

void MainWindow::on_deleteAllButton_clicked()
{
    sceneQWidget->deleteAllShapes();

    updateListWidget();
}

void MainWindow::on_drawCircleButton_clicked()
{
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

void MainWindow::on_changeColorButton_clicked()
{
    qDebug() << Q_FUNC_INFO << "Change Color clicked!!! ";
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();

    qDebug() << "Choosing clolor! " << list.count();

    if (list.count() != 1)
        return;

    QListWidgetItem* item = list.first();

    QColor color = QColorDialog::getColor(Qt::yellow, this);
    if (color.isValid()) {
      qDebug() << "Color Choosen : " << item->text() << " " << qBlue(color.rgb());
      sceneQWidget->updateShapeColor(item, color);
    }
}

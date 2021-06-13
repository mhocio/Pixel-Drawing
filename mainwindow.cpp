#include "mainwindow.h"
#include "ui_mainwindow.h"

using json = nlohmann::json;

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

    //sceneQWidget->run();
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
    sceneQWidget->update();
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

void MainWindow::on_drawRectangleButton_clicked()
{
    sceneQWidget->createNewShape();
    sceneQWidget->setModeDrawRectangle();
    updateStatusBar();
}

void MainWindow::on_fillPolygonColorButton_clicked()
{
    QListWidgetItem* item = getListWidgetSelectedItem();

    if (item == NULL)
        return;

    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if (color.isValid()) {
      sceneQWidget->fillPolygon(item, color);
      sceneQWidget->update();
      // item->setForeground(color);
    }
}

void MainWindow::on_setBoundaryButton_clicked()
{
    clipBoundaryPolygon = getListWidgetSelectedItem();
}

void MainWindow::on_setClipButton_clicked()
{
    polygonToClip = getListWidgetSelectedItem();
}

void MainWindow::on_clipPolygonButton_clicked()
{
    sceneQWidget->createNewPolygon(clipBoundaryPolygon, polygonToClip);
}

void MainWindow::on_fillPolygonwithImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (QString::compare(fileName, QString()) != 0) {
        QImage image;

        if (image.load(fileName) == 0) {
            QMessageBox::warning(this, "Warning", "Cannot open file");
            return;
        }

        QListWidgetItem* item = getListWidgetSelectedItem();

        if (item == NULL)
            return;

        sceneQWidget->fillPolygon(item, image);
        sceneQWidget->update();
    }
}

void MainWindow::on_moveEdgeButton_clicked()
{
    sceneQWidget->setModeMovePolygonEdge();
    updateStatusBar();
}

void MainWindow::on_movePolygonVertexButton_clicked()
{
    sceneQWidget->setModeMovePolygonVertex(getListWidgetSelectedItem());
    updateStatusBar();
}

void MainWindow::on_actionSave_triggered()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Your Project"), "",
                                                 tr("Project File(*.json);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    json j;
    for (auto &shape : sceneQWidget->shapes)
        j.push_back(shape->getJsonFormat());

    std::ofstream file(fileName.toStdString());
    file << j;
}

void MainWindow::on_actionLoad_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.json)"));
    if (fileName.isEmpty())
        return;

    std::ifstream i(fileName.toStdString());
    json j;
    i >> j;

    qDebug() << "wczytano";

    sceneQWidget->shapes.clear();

    for (auto &shape : j) {
        std::unique_ptr<IShape> current = nullptr;
        if (shape["shape"] == "circle") {
            current = std::make_unique<MyCircle>(shape["X"], shape["Y"], shape["radius"]);
        } else if (shape["shape"] == "line") {
            current = std::make_unique<MyLine>(shape["x1"], shape["y1"], shape["x2"], shape["y2"]);
        } else if (shape["shape"] == "pizza") {
            current = std::make_unique<myPizza>(shape["pA"], shape["pB"], shape["pC"]);
        } else if (shape["shape"] == "polygon") {
            current = std::make_unique<myPolygon>(shape["points"]);
            myPolygon* tmpPolygon = dynamic_cast<myPolygon*>(current.get());
            tmpPolygon->setFinished();
        } else if (shape["shape"] == "rectangle") {
            current = std::make_unique<myRectangle>(shape["firstPoint"], shape["secondPoint"]);
        }

        if (shape["shape"] == "rectangle" || shape["shape"] == "polygon") {
            myPolygon* tmpPolygon = dynamic_cast<myPolygon*>(current.get());
            tmpPolygon->isFilled = shape["isFilled"];
            tmpPolygon->isFilledWithColor = shape["isFilledWithColor"];
            tmpPolygon->fillingColor = QColor(shape["fillingColor"][0], shape["fillingColor"][1], shape["fillingColor"][2]);

            tmpPolygon->isFilledWithImage = shape["isFilledWithImage"];
            if (tmpPolygon->isFilledWithImage) {
                qDebug() << shape["fillingImage"].size();
                QByteArray ba;
                //ba = shape["fillingImage"];
                //for (auto elem: shape["fillingImage"])
                  //  ba.append(elem);
                //tmpPolygon->fillingImage = QImage(ba);
            }
        }

        if (current == nullptr)
            continue;

        current->setColor(QColor(shape["color"][0], shape["color"][1], shape["color"][2]));
        current->thickness = shape["thickness"];
        sceneQWidget->shapes.push_back(std::move(current));
    }

    sceneQWidget->update();
}

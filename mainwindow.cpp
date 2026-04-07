#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graph.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // setup ui
    ui->setupUi(this);
    scene = new GraphScene(this);
    connect(ui->CheckBox, &QCheckBox::checkStateChanged, scene, &GraphScene::setHidden);
    connect(ui->addTextButton, &QPushButton::pressed, scene, [&](){scene->addText(ui->input->toPlainText());});
    connect(ui->ResizePushButton, &QPushButton::pressed, this,
            [&](){
                resizeHeight((qreal)ui->HeightInput->toPlainText().toShort());
            });
    connect(ui->IncludeImageButton, &QPushButton::pressed, scene,
            [&](){
                QString path = QFileDialog::getSaveFileName(this, tr("Open Image"), "C:/");
                scene->includeImageWithpath(path);
            });
    width = 1480, height = 680;
    ui->GraphView->setSceneRect(0, 0, width, height);
    ui->GraphView->setScene(scene);
    ui->GraphView->setMouseTracking(true);
    QPen pen;
    pen.setColor(QColor(200, 200, 200));
    pen.setBrush(QColor(200, 200, 200));
    scene->setPen(pen);
    for (int i = 20; i < ui->GraphView->sceneRect().width() - 20; i += 100) {
        for (int j = 20; j < ui->GraphView->sceneRect().height() - 20; j += 100) {
            scene->addFillCircle(i, j, 10);
        }
    }
    scene->setGraph(new Graph());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeHeight(qreal newHeight)
{
    ui->CheckBox->setChecked(false);
    height = newHeight;
    qDebug() << "ok\n";
    scene = new GraphScene();
    connect(ui->CheckBox, &QCheckBox::checkStateChanged, scene, &GraphScene::setHidden);
    ui->GraphView->setScene(scene);
    qDebug() << "ok\n";
    ui->GraphView->setSceneRect(0, 0, width, height);
    QPen pen;
    pen.setColor(QColor(200, 200, 200));
    pen.setBrush(QColor(200, 200, 200));
    scene->setPen(pen);
    for (int i = 20; i < ui->GraphView->sceneRect().width() - 20; i += 100) {
        for (int j = 20; j < ui->GraphView->sceneRect().height() - 20; j += 100) {
            scene->addFillCircle(i, j, 10);
        }
    }
    scene->setGraph(new Graph());
}

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
    connect(ui->addTextButton, &QPushButton::pressed, this, &MainWindow::addTextButtonPushed);
    connect(this, &MainWindow::sendInput, scene, [&](){scene->setInput(ui->input->toPlainText());});
    connect(ui->addTextButton, &QPushButton::pressed, scene, &GraphScene::addText);
    connect(ui->IncludeImageButton, &QPushButton::pressed, scene,
            [&](){
                QString path = QFileDialog::getSaveFileName(this, tr("Open Image"), "C:/");
                scene->includeImageWithpath(path);
            });
    ui->GraphView->setSceneRect(0, 0, 1500, 2000);
    ui->GraphView->setScene(scene);
    ui->GraphView->setMouseTracking(true);
    QPen pen;
    pen.setColor(QColor(200, 200, 200));
    pen.setBrush(QColor(200, 200, 200));
    scene->setPen(pen);
    for (int i = 20; i < 1500 - 20; i += 100) {
        for (int j = 20; j < 2000 - 20; j += 100) {
            scene->addFillCircle(i, j, 10);
        }
    }
    scene->setGraph(new Graph());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTextButtonPushed() {
    emit sendInput();
}

void MainWindow::setInput(QString s) {
    ui->input->setText(s);
}

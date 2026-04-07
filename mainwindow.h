#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QRectF>
#include <QFileDialog>
#include "graphscene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeHeight(qreal newHeight);
signals:
public slots:
private:
    Ui::MainWindow *ui;
    GraphScene *scene;
    qreal width, height;
};
#endif // MAINWINDOW_H

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>


class GraphicsView : QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *par = nullptr);
    ~GraphicsView();


private:
    QGraphicsScene *scene;
};

#endif // GRAPHICSVIEW_H

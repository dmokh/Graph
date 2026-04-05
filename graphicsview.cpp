#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget* par)
    : QGraphicsView(par)
{
    setScene(scene);
    show();
}
GraphicsView::~GraphicsView() {

}

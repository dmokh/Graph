#include "graphscene.h"
#include "edge.h"
#include "circle.h"
#include "vertex.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QTransform>
#include <QtMath>
GraphScene::GraphScene(QObject* parent) : QGraphicsScene(parent) {

}

GraphScene::~GraphScene() {}

void GraphScene::addRect(qreal x, qreal y, qreal weight, qreal height)
{
    QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, weight, height);
    addItem(rect);
}

Circle* GraphScene::addCircle(qreal x, qreal y, qreal r)
{
    Circle* circle = new Circle(x, y, r);
    //circle->setPen(pen);
    addItem(circle);
    field.push_back(circle);
    return circle;
}

Circle* GraphScene::addFillCircle(qreal x, qreal y, qreal r)
{
    Circle* circle = new Circle(x, y, r);
    circle->setPen(pen);
    circle->setBrush(pen.color());
    addItem(circle);
    field.push_back(circle);
    return circle;
}

Vertex* GraphScene::addVertex(qreal x, qreal y, qreal r, bool updateVertexes = true)
{
    Vertex* vertex = new Vertex(x, y, r);
    vertex->setPen(pen);
    addItem(vertex);
    if (updateVertexes) {
        vertexes.push_back(vertex);
    }
    return vertex;
}

Vertex* GraphScene::addFillVertex(qreal x, qreal y, qreal r, bool updateVertexes = true)
{
    Vertex* vertex = new Vertex(x, y, r);
    vertex->setPen(pen);
    vertex->setBrush(pen.color());
    addItem(vertex);
    if (updateVertexes) {
        vertexes.push_back(vertex);
    }
    return vertex;
}


Edge* GraphScene::addEdge(qreal x1, qreal y1, qreal x2, qreal y2, qreal dx, qreal dy) {
    Edge* edge = new Edge(QPointF(x1, y1), QPointF(x2, y2), dx, dy);
    edge->setPen(pen);
    addItem(edge);
    edges.push_back(edge);
    return edge;
}

Text* GraphScene::addTextField(qreal x, qreal y, QString s) {
    Text* text = new Text(x, y, s);
    text_fields.push_back(text);
    addItem(text);
    return text;
}

void GraphScene::setPen(QPen new_pen) {
    pen = new_pen;
}

void GraphScene::setVertex(Vertex *v) {
    attachedVertex = v;
}

Vertex* GraphScene::getVertex() {
    return attachedVertex;
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    QPointF p = e->scenePos();
    for (Circle *c : std::as_const(field)) {
        QRectF Rect = c->boundingRect();
        if (Rect.intersects(QRectF(p.x(), p.y(), 1, 1))) {
            if (!c->isActive()) {
                c->setBoundingScale(2.0, Rect.topLeft().x() - c->getRadius(), Rect.topLeft().y() - c->getRadius());
                c->activate();
            }
        } else if (c->isActive()) {
            c->setBoundingScale(1, Rect.topLeft().x() + c->getRadius(), Rect.topLeft().y() + c->getRadius());
            c->disactivate();
        }
    }
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    if (e->buttons() == Qt::LeftButton) {
        QPointF p = e->scenePos();
        for (Vertex *v : std::as_const(vertexes)) {
            QRectF Rect = v->boundingRect();
            if (Rect.intersects(QRectF(p.x(), p.y(), 1, 1))) {
                if (isAttached) {
                    removeItem(attachedVertex->getColoredVertex());
                    attachedVertex->setColoredVertex(nullptr);
                    isAttached = false;
                    attachedVertex = nullptr;
                    return;
                }
                qreal x = v->getCenter().x(), y = v->getCenter().y();
                QVector<Edge* > for_delete;
                for (Edge *edge : v->getEdges()) {
                    v->deleteEdge(edge);
                    for (Vertex *u : v->getNeighbors()) {
                        if (u->getEdges().contains(edge)) {
                            u->deleteEdge(edge);
                        }
                    }
                    removeItem(edge);
                    edges.removeOne(edge);
                }
                for (Text* text : std::as_const(text_fields)) {
                    if (text->pos().x() + 2 + 3 * (text->toPlainText().size() - 1) == v->getCenter().x() && text->pos().y() + 8 == v->getCenter().y()) {
                        text_fields.removeOne(text);
                        removeItem(text);
                    }
                }
                graph->removeVertex(v);
                removeItem(v);
                vertexes.removeOne(v);
                QPen new_pen;
                new_pen.setColor(QColor(200, 200, 200));
                setPen(new_pen);
                Circle* c = addFillCircle(x - 10, y - 10, 10);
                c->setVisible(!isHidden);
                return;
            }
        }
        for (Circle *c : std::as_const(field)) {
            QRectF Rect = c->boundingRect();
            if (Rect.intersects(QRectF(p.x(), p.y(), 1, 1))) {
                QPen new_pen;
                new_pen.setColor(QColor(0, 0, 0));
                setPen(new_pen);
                Vertex *v = addVertex(c->getCenter().x()-30, c->getCenter().y()-30, 30);
                graph->addVertex(v);
                removeItem(c);
                field.removeOne(c);
                break;
            }
        }
    } else if (e->buttons() == Qt::RightButton) {
        QPointF p = e->scenePos();
        for (Vertex *v : std::as_const(vertexes)) {
            QRectF Rect = v->boundingRect();
            if (Rect.intersects(QRectF(p.x(), p.y(), 1, 1))) {
                if (!isAttached) {
                    isAttached = true;
                    QPen newPen;
                    newPen.setColor(QColor(255, 0, 0));
                    setPen(newPen);
                    attachedVertex = v;
                    Vertex* coloredVertex = addFillVertex(v->getCenter().x()-v->getRadius(), v->getCenter().y()-v->getRadius(), v->getRadius(), false);
                    v->setColoredVertex(coloredVertex);
                } else if (v != attachedVertex && !v->getNeighbors().contains(attachedVertex)) {
                    QPen newPen;
                    newPen.setColor(QColor(0, 0, 0));
                    newPen.setWidth(1);
                    setPen(newPen);
                    qreal x = v->getCenter().x() - attachedVertex->getCenter().x();
                    qreal y = v->getCenter().y() - attachedVertex->getCenter().y();
                    Edge* new_edge = addEdge(attachedVertex->getCenter().x() + 30 * x / qSqrt(x*x + y*y),
                                             attachedVertex->getCenter().y() + 30 * y / qSqrt(x*x + y*y),
                                             v->getCenter().x() - 30 * x / qSqrt(x*x + y*y),
                                             v->getCenter().y() - 30 * y / qSqrt(x*x + y*y),
                                             v->getCenter().x() - attachedVertex->getCenter().x(),
                                             v->getCenter().y() - attachedVertex->getCenter().y());
                    v->addEdge(new_edge);
                    attachedVertex->addEdge(new_edge);
                    v->addNeighbor(attachedVertex);
                    attachedVertex->addNeighbor(v);

                    removeItem(attachedVertex->getColoredVertex());
                    attachedVertex->setColoredVertex(nullptr);
                    isAttached = false;
                    attachedVertex = nullptr;
                }
                return;
            }
        }
        for (Edge *edge : std::as_const(edges)) {
            if (edge->intersection(e->scenePos())) {
                for (Vertex* v : std::as_const(vertexes)) {
                    if (v->getEdges().contains(edge)) {
                        for (Vertex* u : v->getNeighbors()) {
                            if (u->getEdges().contains(edge)) {
                                v->deleteNeighbor(u);
                                u->deleteNeighbor(v);
                            }
                        }
                        v->deleteEdge(edge);
                    }
                }
                removeItem(edge);
                edges.removeOne(edge);
            }
        }
    }
}

void GraphScene::setGraph(Graph* gr) {
    graph = gr;
}

Graph* GraphScene::getGraph() {
    return graph;
}

void GraphScene::setHidden(bool isHidden_) {
    isHidden = isHidden_;
    for (Circle *c : std::as_const(field)) {
        c->setVisible(!isHidden);
    }
}
void GraphScene::addText(QString input) {
    if (isAttached) {
        for (Text* text : std::as_const(text_fields)) {
            if (text->pos().x() + 2 + 3 * (text->toPlainText().size() - 1) == attachedVertex->getCenter().x() && text->pos().y() + 8 == attachedVertex->getCenter().y()) {
                text_fields.removeOne(text);
                removeItem(text);
            }
        }
        Text* text = addTextField(attachedVertex->getCenter().x() - 2 - 3 * (input.size() - 1), attachedVertex->getCenter().y()-8, input);
        text->setFont(QFont("times", 8));

        removeItem(attachedVertex->getColoredVertex());
        attachedVertex->setColoredVertex(nullptr);
        isAttached = false;
        attachedVertex = nullptr;
    }
}
void GraphScene::setInput(QString s) {
    input = s;
}

void GraphScene::includeImageWithpath(QString path)
{
    this->clearSelection();
    this->setSceneRect(this->itemsBoundingRect());
    QImage image(this->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    this->render(&painter);
    image.save(path);
}

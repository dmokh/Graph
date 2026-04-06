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
                if (!isHidden) {
                    QPen new_pen;
                    new_pen.setColor(QColor(200, 200, 200));
                    setPen(new_pen);
                    addFillCircle(x - 10, y - 10, 10);
                }
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
                    QPen pen;
                    pen.setColor(QColor(255, 0, 0));
                    setPen(pen);
                    attachedVertex = v;
                    Vertex* coloredVertex = addFillVertex(v->getCenter().x()-v->getRadius(), v->getCenter().y()-v->getRadius(), v->getRadius(), false);
                    v->setColoredVertex(coloredVertex);
                } else if (v != attachedVertex && !v->getNeighbors().contains(attachedVertex)) {
                    QPen pen;
                    pen.setColor(QColor(0, 0, 0));
                    setPen(pen);
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
    if (isHidden_) {
        for (Circle *c : std::as_const(field)) {
            removeItem(c);
            field.removeOne(c);
        }
    } else {
        QPen pen;
        pen.setColor(QColor(200, 200, 200));
        pen.setBrush(QColor(200, 200, 200));
        setPen(pen);
        qDebug() << sceneRect().width();
        for (int i = 20; i < 1480 - 20; i += 100) {
            for (int j = 20; j < 1500 - 20; j += 100) {
                bool fl = false;
                for (Vertex *v : std::as_const(vertexes)) {
                    if (QPointF(i+10, j+10) == v->getCenter()) {
                        fl = true;
                        break;
                    }
                }
                if (!fl) {
                    addFillCircle(i, j, 10);
                }
            }
        }
        pen.setColor(QColor(0, 0, 0));
        setPen(pen);
        QVector<Edge* > new_edges;
        for (Edge* edge : std::as_const(edges)) {
            removeItem(edge);
            new_edges.push_back(addEdge(edge->getBegin().x(), edge->getBegin().y(), edge->getEnd().x(), edge->getEnd().y(), edge->getEnd().x() - edge->getBegin().x(), edge->getEnd().y()-edge->getBegin().y()));
        }
        edges = new_edges;
    }
    isHidden = isHidden_;
}
void GraphScene::addText() {
    if (isAttached) {
        for (Text* text : std::as_const(text_fields)) {
            if (text->pos().x() + 2 + 3 * (text->toPlainText().size() - 1) == attachedVertex->getCenter().x() && text->pos().y() + 8 == attachedVertex->getCenter().y()) {
                text_fields.removeOne(text);
                removeItem(text);
            }
        }
        Text* text = addTextField(attachedVertex->getCenter().x() - 2 - 3 * (input.size() - 1), attachedVertex->getCenter().y()-8, input);
        text->setFont(QFont("times", 8));
        QPen pen;
        pen.setColor(QColor(0, 0, 0));
        setPen(pen);
        Vertex *v = addVertex(attachedVertex->getCenter().x()-attachedVertex->getRadius(), attachedVertex->getCenter().y()-attachedVertex->getRadius(), attachedVertex->getRadius());
        graph->addVertex(v);
        removeItem(attachedVertex);
        vertexes.removeOne(attachedVertex);
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

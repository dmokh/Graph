#ifndef VERTEX_H
#define VERTEX_H
#include <QVector>
#include <QGraphicsEllipseItem>
#include "edge.h"
class Vertex : public QGraphicsEllipseItem
{
public:
    Vertex();
    Vertex(qreal x, qreal y, qreal r);
    ~Vertex();
    void addNeighbor(Vertex* neighbor);
    void deleteNeighbor(Vertex* neighbor);
    void addEdge(Edge* e);
    void deleteEdge(Edge* e);
    void clearEdges();
    QVector<Vertex *> getNeighbors();
    QVector<Edge* > getEdges();
    QRectF boundingRect() const override;
    QPointF getCenter();
    qreal getRadius();
private:
    QVector<Edge* > edges;
    QVector<Vertex *> neighbors;
    qreal radius;
    QPointF center;
    QRectF boundingrect;
};

#endif // VERTEX_H

#include "vertex.h"
#include <QDebug>
Vertex::Vertex() {};
Vertex::Vertex(qreal x, qreal y, qreal r)
    : QGraphicsEllipseItem(x, y, 2 * r, 2 * r),
    center(x+r, y+r), radius(r), boundingrect(x-0.5, y-0.5, 2 * r+1, 2 * r+1) {
}
Vertex::~Vertex() {};
void Vertex::addNeighbor(Vertex* neighbor) {
    neighbors.push_back(neighbor);
}
void Vertex::deleteNeighbor(Vertex* neighbor) {
    neighbors.removeOne(neighbor);
}
QVector<Vertex* > Vertex::getNeighbors() {
    return neighbors;
}
void Vertex::addEdge(Edge* e) {
    edges.push_back(e);
}
void Vertex::deleteEdge(Edge* e) {
    edges.removeOne(e);
}
void Vertex::clearEdges() {
    edges.clear();
}
QVector<Edge* > Vertex::getEdges() {
    return edges;
}
QRectF Vertex::boundingRect() const {
    return boundingrect;
}
QPointF Vertex::getCenter() {
    return center;
}
qreal Vertex::getRadius() {
    return radius;
}


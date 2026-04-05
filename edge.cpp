#include "edge.h"

Edge::Edge()
    : QGraphicsLineItem(0, 0, 0, 0), begin(0, 0), end(0, 0), weight(1), dx(0), dy(0) {}

Edge::Edge(QPointF b, QPointF e, qreal dx_, qreal dy_)
    : QGraphicsLineItem(b.x(), b.y(), e.x(), e.y()), begin(b), end(e), weight(1), dx(dx_), dy(dy_) {}

Edge::Edge(QPointF b, QPointF e, qreal weight_, qreal dx_, qreal dy_)
    : QGraphicsLineItem(b.x(), b.y(), e.x(), e.y()), begin(b), end(e), weight(weight_), dx(dx_), dy(dy_) {}
Edge::~Edge() {};
void Edge::setWeight(qreal newWeight) {
    weight = newWeight;
}
void Edge::setOrientation(bool orientation) {
    isOriented = orientation;
}
qreal Edge::getDX() {
    return dx;
}
qreal Edge::getDY() {
    return dy;
}
QPointF Edge::getBegin() {
    return begin;
}
QPointF Edge::getEnd() {
    return end;
}
bool Edge::intersection(QPointF p) {
    return ((((p.y() - begin.y()) / (p.x() - begin.x()) - (end.y() - p.y()) / (end.x() - p.x())) < 0.01) &&
            (qMin(begin.x(), end.x()) <= p.x() && qMax(begin.x(), end.x()) >= p.x()) &&
            (qMin(begin.y(), end.y()) <= p.y() && qMax(begin.y(), end.y()) >= p.y()));
}

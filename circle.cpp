#include "circle.h"
Circle::Circle() {

}
Circle::Circle(qreal x, qreal y, qreal r)
    : QGraphicsEllipseItem(x, y, 2 * r, 2 * r)
{
    boundingrect = QRectF(x-0.5, y-0.5, 2 * r + 1, 2 * r + 1);
    center = QPointF(x + r, y + r);
    radius = r;
    baseRadius = radius;
}
Circle::~Circle() {

}
QRectF Circle::boundingRect() const {
    return boundingrect;
}
void Circle::setBoundingScale(qreal x, qreal xc, qreal yc) {
    this->setTransformOriginPoint(center);
    this->setScale(x);
    radius = x * baseRadius;
    boundingrect = QRectF(center.x() - radius - 0.5, center.y() - radius - 0.5, 2 * radius + 1, 2 * radius + 1);
}
void Circle::activate() {
    isActivated = true;
}
void Circle::disactivate() {
    isActivated = false;
}
bool Circle::isActive() {
    return isActivated;
};
QPointF Circle::getCenter() {
    return center;
}
qreal Circle::getRadius() {
    return radius;
}

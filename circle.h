#ifndef CIRCLE_H
#define CIRCLE_H
#include <QGraphicsItem>
#include <QRectF>
class Circle : public QGraphicsEllipseItem
{
public:
    Circle();
    Circle(qreal x, qreal y, qreal r);
    ~Circle();
    QRectF boundingRect() const override;
    void setBoundingScale(qreal x, qreal xc, qreal yc);
    void activate();
    void disactivate();
    bool isActive();
    QPointF getCenter();
    qreal getRadius();
private:
    QRectF boundingrect;
    QPointF center;
    qreal radius;
    qreal baseRadius;
    bool isActivated = false;
};

#endif // CIRCLE_H

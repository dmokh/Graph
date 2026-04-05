#ifndef EDGE_H
#define EDGE_H
#include <QObject>
#include <QGraphicsLineItem>
#include <QPointF>
class Edge : public QGraphicsLineItem
{
public:
    Edge();
    Edge(QPointF b, QPointF e, qreal dx_, qreal dy_);
    Edge(QPointF b, QPointF e, qreal weight_, qreal dx_, qreal dy_);
    ~Edge();
    void setWeight(qreal newWeight);
    void setOrientation(bool orientation);
    qreal getDX(), getDY();
    QPointF getBegin();
    QPointF getEnd();
    bool intersection(QPointF rect);

private:
    QPointF begin, end;
    qreal weight, dx, dy;
    bool isOriented;
    static inline qreal radius = 30;
};

#endif // EDGE_H

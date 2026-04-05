#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QCursor>
#include <QPainter>
#include "circle.h"
#include "graph.h"
#include "text.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = nullptr);
    ~GraphScene();
    void addRect(qreal x, qreal y, qreal width, qreal height);
    Circle* addCircle(qreal x, qreal y, qreal r);
    Circle* addFillCircle(qreal x, qreal y, qreal r);
    Vertex* addVertex(qreal x, qreal y, qreal r);
    Vertex* addFillVertex(qreal x, qreal y, qreal r);
    Text* addTextField(qreal x, qreal y, QString s);
    Edge* addEdge(qreal x1, qreal y1, qreal x2, qreal y2, qreal dx, qreal dy);
    void setPen(QPen new_pen);
    void setGraph(Graph* gr);
    Graph* getGraph();
    void setVertex(Vertex *v);
    Vertex* getVertex();
    void setInput(QString s);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
signals:
    void inputChanged();
public slots:
    void setHidden(bool isHidden);
    void addText();
    void includeImageWithpath(QString path);
private:
    QVector<Circle *> field;
    QVector<Edge *> edges;
    QVector<Vertex *> vertexes;
    QVector<Text *> text_fields;
    QString input;
    QPen pen;
    Graph* graph;
    static inline Circle* attachedCircle = nullptr;
    static inline bool isHidden = false;
    static inline bool isAttached = false;
    static inline Vertex* attachedVertex = nullptr;

};

#endif // GRAPHSCENE_H

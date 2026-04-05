#ifndef GRAPH_H
#define GRAPH_H
#include <QVector>
#include "edge.h"
#include "vertex.h"
class Graph : QObject
{
    Q_OBJECT
public:
    Graph();
    ~Graph();
    void addVertex(Vertex *v);
    Edge* addEdge(Vertex *u, Vertex *v);
    void removeVertex(Vertex *v);
    void removeEdge(Edge *e);
    QVector<Vertex *> getVertexes();
    QVector<Edge *> getEdges();
    void setOrientation(bool orientation);
private:
    QVector<Vertex *> Vertexes;
    QVector<Edge *> Edges;
    bool isOriented = false;
};

#endif // GRAPH_H

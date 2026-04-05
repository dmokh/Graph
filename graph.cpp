#include "graph.h"

Graph::Graph() {}
Graph::~Graph() {}
void Graph::addVertex(Vertex *v) {
    Vertexes.push_back(v);
}
Edge* Graph::addEdge(Vertex *v, Vertex *u) {
    if (!Vertexes.contains(v)) {
        Vertexes.push_back(v);
    }
    if (!Vertexes.contains(u)) {
        Vertexes.push_back(u);
    }
    Edge *e = new Edge(v->getCenter(), u->getCenter(), u->getCenter().x()-v->getCenter().x(), u->getCenter().y()-v->getCenter().y()); // fix centers into boards
    Edges.push_back(e);
    return e;
}
void Graph::setOrientation(bool orientation) {
    isOriented = orientation;
    for (Edge *e : std::as_const(Edges)) {
        e->setOrientation(orientation);
    }
}
QVector<Vertex *> Graph::getVertexes() {
    return Vertexes;
}
QVector<Edge *> Graph::getEdges() {
    return Edges;
}
void Graph::removeVertex(Vertex *v) {
    if (Vertexes.contains(v)) {
        Vertexes.removeOne(v);
    } else {
        qWarning() << "Removing Error: cannot find vertex expected to remove";
    }
    // fix
    // for (Edge *e : std::as_const(Edges)) {
    //     if (e->getVertexes().first == v || e->getVertexes().second == v) {
    //         Edges.removeOne(e);
    //     }
    // }
}
void Graph::removeEdge(Edge *e) {
    if (Edges.contains(e)) {
        Edges.removeOne(e);
    } else {
        qWarning() << "Removing Error: cannot find vertex expected to remove";
    }
}

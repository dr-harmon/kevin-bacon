#pragma once

#include "graph.h"

template <typename VertexElement, typename EdgeElement>
class BFS {
private:

    struct VertexData {
        bool visited;
        VertexElement element;
    };

    struct EdgeData {
        bool visited;
        EdgeElement element;
    };

    typedef Graph<VertexData,EdgeData> G;
    
    G graph;

public:

    typedef typename G::Vertex Vertex;
    typedef typename G::Edge Edge;

    Vertex& insertVertex(const VertexElement& element) {
        return graph.insertVertex({false, element});
    }

    Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeElement& element) {
        return graph.insertEdge(v, u, {false, element});
    }

protected:

    void initialize() {
        for (auto pv = graph.vertices().begin(); pv != graph.vertices().end(); ++pv) {
            unvisit(*pv);
        }

        for (auto pe = graph.edges().begin(); pe != graph.edges().end(); ++pe) {
            unvisit(*pe);
        }
    }

    void traverse(Vertex& root) {
        // TODO
    }

    virtual void startVisit(Vertex& v) {}
    virtual void traverseDiscovery(Edge& e, Vertex& from) {}
    virtual bool isDone() const { return false; }

    void visit(Vertex& v) {
        (*v).visited = true;
    }

    void visit(Edge& e) {
        (*e).visited = true;
    }

    void unvisit(Vertex& v) {
        (*v).visited = false;
    }

    void unvisit(Edge& e) {
        (*e).visited = false;
    }

    bool isVisited(Vertex& v) {
        return (*v).visited;
    }

    bool isVisited(Edge& e) {
        return (*e).visited;
    }
};

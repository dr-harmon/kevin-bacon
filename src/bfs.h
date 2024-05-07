#pragma once

#include "graph.h"

#include <queue>

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
    typedef typename G::VertexList VertexList;
    typedef typename G::VertexItor VertexItor;
    typedef typename G::EdgeList EdgeList;
    typedef typename G::EdgeItor EdgeItor;
    
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
        VertexList verts = graph.vertices();
        for (VertexItor pv = verts.begin(); pv != verts.end(); ++pv) {
            unvisit(*pv);
        }

        EdgeList edges = graph.edges();
        for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe) {
            unvisit(*pe);
        }
    }

    void traverse(Vertex& root) {
        initialize();
        std::queue<Vertex> queue;
        startVisit(root);
        visit(root);
        queue.push(root);
        while (!queue.empty() && !isDone()) {
            Vertex v = queue.front();
            queue.pop();
            EdgeList edges = v.incidentEdges();
            for (EdgeItor pe = edges.begin(); pe != edges.end(); ++pe) {
                Edge& edge = *pe;
                if (!isVisited(edge)) {
                    visit(edge);
                    Vertex& w = edge.opposite(v);
                    if (!isVisited(w)) {
                        traverseDiscovery(edge, v);
                        startVisit(w);
                        visit(w);
                        queue.push(w);
                    }
                }
            }
        }
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

#pragma once

#include <ranges>
#include <stdexcept>
#include <unordered_map>
#include <vector>

template <typename VertexElement, typename EdgeElement>
class Graph {
public:
    class Vertex;
    class Edge;

protected:
    typedef std::unordered_map<int,Vertex> VertexMap;
    typedef std::unordered_map<int,Edge> EdgeMap;
    // In this implementation, the adjacency list maps a vertex ID to
    // the IDs of its incident edges.
    typedef std::unordered_map<int,std::vector<int>> AdjacencyList;

public:

    class EdgeItor;
    class EdgeList {
    public:
        EdgeItor begin() {
            return EdgeItor(edges.begin(), edgeMap);
        }

        EdgeItor end() {
            return EdgeItor(edges.end(), edgeMap);
        }

    private:
        std::vector<int> edges;
        EdgeMap *edgeMap;
        friend Graph;

        EdgeList(std::vector<int>& _edges, EdgeMap *_edgeMap) : edges(_edges), edgeMap(_edgeMap) {}
    };

    class EdgeItor {
    public:
        Edge& operator*() const {
            return edgeMap->at(*it);
        }

        EdgeItor& operator++() {
            ++it;
            return *this;
        }

        EdgeItor operator++(int) {
            EdgeItor temp = *this;
            ++it;
            return temp;
        }

        bool operator!=(const EdgeItor& other) const {
            return it != other.it;
        }

    private:
        std::vector<int>::iterator it;
        EdgeMap *edgeMap;
        friend EdgeList;

        EdgeItor(std::vector<int>::iterator _it, EdgeMap *_edgeMap) : it(_it), edgeMap(_edgeMap)  {}
    };

    class Vertex {
    public:
        Vertex() : element(VertexElement{}), id(-1), adjacencyList(nullptr), edgeMap(nullptr) {}

        bool isAdjacentTo(const Vertex& v) const {
            for (int edgeID : adjacencyList->at(id)) {
                Edge edge = edgeMap->at(edgeID);
                if ((id == edge.startVertexID && v.id == edge.endVertexID) ||
                    (id == edge.endVertexID && v.id == edge.startVertexID)) {
                    return true;
                }
            }
            return false;
        }

        EdgeList incidentEdges() const {
            return EdgeList(adjacencyList->at(id), edgeMap);
        }

        EdgeList outEdges() const {
            std::vector<int> edges;
            for (int edgeID : adjacencyList->at(id)) {
                Edge& edge = edgeMap->at(edgeID);
                if (edge.isDirected() && edge.origin().id == id) {
                    edges.push_back(edgeID);
                }
            }
            return EdgeList(edges, edgeMap);
        }

        const VertexElement& operator*() const {
            return element;
        }

        VertexElement& operator*() {
            return element;
        }

        bool operator ==(const Vertex& other) const {
            return id == other.id;
        }

        struct Hash {
            size_t operator()(const Vertex& v) const {
                return std::hash<int>()(v.id);
            }
        };

    public:
        VertexElement element;
        int id;
        AdjacencyList *adjacencyList;
        EdgeMap *edgeMap;
        friend Graph;

        Vertex(const VertexElement& _element, int _id, EdgeMap *_edgeMap, AdjacencyList *_adjacencyList) :
            element(_element), id(_id), edgeMap(_edgeMap), adjacencyList(_adjacencyList) {}
    };

    class Edge {
    public:
        Edge() : element(EdgeElement{}), id(-1), startVertexID(-1), endVertexID(-1), vertexMap(nullptr) {}

        std::pair<Vertex,Vertex> endVertices() const {
            return std::make_pair(vertexMap->at(startVertexID), vertexMap->at(endVertexID));
        }

        const Vertex& opposite(const Vertex& v) const {
            return vertexMap->at(v.id == startVertexID ? endVertexID : startVertexID);
        }

        Vertex& opposite(const Vertex& v) {
            return vertexMap->at(v.id == startVertexID ? endVertexID : startVertexID);
        }

        Vertex& origin() {
            return vertexMap->at(startVertexID);
        }

        Vertex& dest() {
            return vertexMap->at(endVertexID);
        }

        bool isIncidentOn(const Vertex& v) const {
            return v.id == startVertexID || v.id == endVertexID;
        }

        bool isDirected() const {
            return directed;
        }

        const EdgeElement& operator*() const {
            return element;
        }

        EdgeElement& operator*() {
            return element;
        }

    private:
        EdgeElement element;
        int id;
        int startVertexID, endVertexID;
        bool directed;
        VertexMap *vertexMap;
        friend Graph;

        Edge(const EdgeElement& _element, int _id, int _startVertexID, int _endVertexID, bool _directed, VertexMap *_vertexMap) :
            element(_element), id(_id), startVertexID(_startVertexID), endVertexID(_endVertexID), directed(_directed), vertexMap(_vertexMap) {}
    };

    Vertex& insertVertex(const VertexElement& element) {
        Vertex v(element, nextVertexID++, &edgeMap, &adjacencyList);
        vertexMap.emplace(v.id, v);
        adjacencyList.emplace(v.id, std::vector<int>{});
        return vertexMap.at(v.id);
    }

    Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeElement& element) {
        return insertEdge(v, u, element, false);
    }

    Edge& insertDirectedEdge(const Vertex& v, const Vertex& u, const EdgeElement& element) {
        return insertEdge(v, u, element, true);
    }

    void eraseVertex(const Vertex& v) {
        vertexMap.erase(v.id);
        for (int edgeID : adjacencyList.at(v.id)) {
            edgeMap.erase(edgeID);
        }
    }

    void eraseEdge(const Edge& e) {
        edgeMap.erase(e.id);
        auto startEdges = adjacencyList.at(e.startVertexID);
        startEdges.erase(std::remove(startEdges.begin(), startEdges.end(), e.id), startEdges.end());
        auto endEdges = adjacencyList.at(e.endVertexID);
        endEdges.erase(std::remove(endEdges.begin(), endEdges.end(), e.id), endEdges.end());
    }

    auto vertices() const {
        return std::views::values(vertexMap);
    }

    auto vertices() {
        return std::views::values(vertexMap);
    }

    auto edges() const {
        return std::views::values(edgeMap);
    }

    auto edges() {
        return std::views::values(edgeMap);
    }

private:
    int nextVertexID = 1;
    int nextEdgeID = 1;
    VertexMap vertexMap;
    EdgeMap edgeMap;
    AdjacencyList adjacencyList;

    Edge& insertEdge(const Vertex& v, const Vertex& u, const EdgeElement& element, bool directed) {
        if (!vertexMap.contains(v.id) || !vertexMap.contains(u.id)) {
            throw std::runtime_error("Cannot add edge between unknown vector");
        }

        Edge e(element, nextEdgeID++, v.id, u.id, directed, &vertexMap);
        edgeMap.emplace(e.id, e);
        adjacencyList.at(v.id).push_back(e.id);
        adjacencyList.at(u.id).push_back(e.id);
        return edgeMap.at(e.id);
    }
};

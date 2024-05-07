#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "bfs.h"

class BaconFinder : public BFS<std::string,int> {

private:

    std::string baconID;
    Vertex sourceVertex;
    Vertex targetVertex;
    std::unordered_map<std::string,Vertex> vertexMap;
    std::unordered_map<Vertex,Vertex,Vertex::Hash> parents;
    bool done = false;

protected:

    void startVisit(Vertex& v) {
        if (v == targetVertex) {
            done = true;
        }
    }

    void traverseDiscovery(Edge& e, Vertex& from) {
        parents.emplace(e.opposite(from), from);
    }

    bool isDone() const {
        return done;
    }

public:

    BaconFinder(std::string _baconID) : baconID(_baconID), sourceVertex(insertVertex(_baconID)) {
        vertexMap.emplace(baconID, sourceVertex);
    }

    void addRelationship(const std::string& movie, const std::string& actor) {
        Vertex movieVertex = vertexMap.contains(movie) ? vertexMap.at(movie) : insertVertex(movie);
        Vertex actorVertex = vertexMap.contains(actor) ? vertexMap.at(actor) : insertVertex(actor);
        insertEdge(movieVertex, actorVertex, 0);
        vertexMap.emplace(movie, movieVertex);
        vertexMap.emplace(actor, actorVertex);
    }

    std::vector<std::string> getBaconPath(const std::string& actor) {
        std::vector<std::string> path;
        targetVertex = vertexMap.at(actor);
        traverse(sourceVertex);
        Vertex parent = vertexMap.at(actor);
        while (parents.contains(parent)) {
            path.push_back((*parent).element);
            parent = parents.at(parent);
        }
        path.push_back(baconID);
        return path;
    }
};

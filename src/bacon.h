#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "bfs.h"

class BaconFinder : public BFS<std::string,int> {

private:

    std::string baconID;
    Vertex& baconVertex;
    std::unordered_map<std::string,Vertex> vertexMap;
    std::unordered_map<std::string,std::string> parents;
    std::string targetActor;
    bool done;

protected:

    void startVisit(Vertex& v) {
        if ((*v).element == targetActor) {
            done = true;
        }
    }

    void traverseDiscovery(Edge& e, Vertex& from) {
        auto v = (*from).element;
        auto w = (*e.opposite(from)).element;
        parents.emplace(w, v);
    }

    bool isDone() const {
        return done;
    }

public:

    BaconFinder(std::string _baconID) : baconID(_baconID), baconVertex(insertVertex(_baconID)) {
        vertexMap.emplace(baconID, baconVertex);
    }

    void addRelationship(const std::string& movie, const std::string& actor) {
        Vertex movieVertex = vertexMap.count(movie) > 0 ? vertexMap.at(movie) : insertVertex(movie);
        Vertex actorVertex = vertexMap.count(actor) > 0 ? vertexMap.at(actor) : insertVertex(actor);
        insertEdge(movieVertex, actorVertex, 0);
        vertexMap.emplace(movie, movieVertex);
        vertexMap.emplace(actor, actorVertex);
    }

    std::vector<std::string> getBaconPath(const std::string& actor) {
        std::vector<std::string> path;
        targetActor = actor;
        traverse(baconVertex);
        auto parent = actor;
        while (parents.count(parent) > 0) {
            path.push_back(parent);
            parent = parents[parent];
        }
        path.push_back(baconID);
        return path;
    }
};

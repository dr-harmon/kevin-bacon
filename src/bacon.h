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

        // TODO

        return path;
    }
};

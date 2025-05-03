#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "bfs.h"

class BaconFinder : public BFS<std::string,int> {

private:

    std::string baconID;
    Vertex sourceVertex;
    std::unordered_map<std::string,Vertex> vertexMap;

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

        // TODO

        return path;
    }
};

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "bfs.h"

using namespace std;

class BFSTest : public BFS<std::string,int> {

    virtual void startVisit(Vertex& v) {
        vertexVisits.push_back((*v).element);
    }

    virtual void traverseDiscovery(Edge& e, Vertex& from) {
		Vertex to = e.opposite(from);
        discoveredEdges.push_back({(*from).element, (*to).element});
    }

public:

    vector<string> vertexVisits;
    vector<pair<string,string>> discoveredEdges;

    void search(Vertex& v) {
		traverse(v);
	}
};

TEST_CASE("BFS traversal")
{
	BFSTest bfs;
	auto& frankfurt = bfs.insertVertex("Frankfurt");
	auto mannheim = bfs.insertVertex("Mannheim");
	auto karlsruhe = bfs.insertVertex("Karlsruhe");
	auto augsburg = bfs.insertVertex("Augsburg");
	auto munchen = bfs.insertVertex("München");
	auto wurzburg = bfs.insertVertex("Würzburg");
	auto erfurt = bfs.insertVertex("Erfurt");
	auto stuttgart = bfs.insertVertex("Stuttgart");
	auto nurnberg = bfs.insertVertex("Nürnberg");
	auto kassel = bfs.insertVertex("Kassel");
	bfs.insertEdge(frankfurt, mannheim, 0);
	bfs.insertEdge(mannheim, karlsruhe, 0);
	bfs.insertEdge(karlsruhe, augsburg, 0);
	bfs.insertEdge(augsburg, munchen, 0);
	bfs.insertEdge(munchen, nurnberg, 0);
	bfs.insertEdge(munchen, kassel, 0);
	bfs.insertEdge(kassel, frankfurt, 0);
	bfs.insertEdge(frankfurt, wurzburg, 0);
	bfs.insertEdge(wurzburg, erfurt, 0);
	bfs.insertEdge(wurzburg, nurnberg, 0);
	bfs.insertEdge(nurnberg, stuttgart, 0);

    bfs.search(frankfurt);

    REQUIRE(bfs.vertexVisits == vector<string> {
        "Frankfurt",
        "Mannheim",
        "Kassel",
        "Würzburg",
        "Karlsruhe",
        "München",
        "Erfurt",
        "Nürnberg",
        "Augsburg",
        "Stuttgart",
    });
    REQUIRE(bfs.discoveredEdges == vector<pair<string,string>> {
        {"Frankfurt", "Mannheim"},
        {"Frankfurt", "Kassel"},
        {"Frankfurt", "Würzburg"},
        {"Mannheim", "Karlsruhe"},
        {"Kassel", "München"},
        {"Würzburg", "Erfurt"},
        {"Würzburg", "Nürnberg"},
        {"Karlsruhe", "Augsburg"},
        {"Nürnberg", "Stuttgart"},
    });
}

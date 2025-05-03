#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "graph.h"

#include <ranges>
#include <set>
#include <iostream>
using namespace std;

Graph<int,char> makeDefaultGraph()
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    return graph;
}

set<char> getVertexElements(const std::ranges::forward_range auto& vertices)
{
    set<char> vertexSet;
    for (auto vertex : vertices) {
        vertexSet.insert(*vertex);
	}
    return vertexSet;
}

set<char> getEdgeElements(const std::ranges::forward_range auto& edges)
{
    set<char> edgeSet;
    for (auto edge : edges) {
        edgeSet.insert(*edge);
	}
    return edgeSet;
}

set<char> getEdgeElements(Graph<int,char>::EdgeList edgeList)
{
    set<char> edgeSet;
    for (auto it = edgeList.begin(); it != edgeList.end(); ++it) {
        edgeSet.insert(**it);
	}
    return edgeSet;
}

TEST_CASE("Insert vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');

    REQUIRE(*v1 == 'a');
    REQUIRE(*v2 == 'b');
    REQUIRE(*v3 == 'c');
}

TEST_CASE("Insert edge")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(*e1 == 'm');
    REQUIRE(*e2 == 'n');
}

TEST_CASE("Number of vertices")
{
	auto graph = makeDefaultGraph();
	REQUIRE(graph.vertices().size() == 3);
	REQUIRE(graph.edges().size() == 2);
}

TEST_CASE("Erase vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

    graph.eraseVertex(v2);

	auto vertexSet = getVertexElements(graph.vertices());
    REQUIRE(vertexSet.size() == 2);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('c') == 1);

	auto edgeSet = getEdgeElements(graph.edges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('n') == 1);
}

TEST_CASE("Erase edge")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

    graph.eraseEdge(e1);

	auto vertexSet = getVertexElements(graph.vertices());
    REQUIRE(vertexSet.size() == 3);
    REQUIRE(vertexSet.count('a') == 1);
    REQUIRE(vertexSet.count('b') == 1);
    REQUIRE(vertexSet.count('c') == 1);

	auto edgeSet = getEdgeElements(graph.edges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('n') == 1);
}

TEST_CASE("Vertex is adjacent to")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE_FALSE(v1.isAdjacentTo(v1));
    REQUIRE(v1.isAdjacentTo(v2));
    REQUIRE(v1.isAdjacentTo(v3));
    REQUIRE(v2.isAdjacentTo(v1));
    REQUIRE_FALSE(v2.isAdjacentTo(v2));
    REQUIRE_FALSE(v2.isAdjacentTo(v3));
    REQUIRE(v3.isAdjacentTo(v1));
    REQUIRE_FALSE(v3.isAdjacentTo(v2));
    REQUIRE_FALSE(v3.isAdjacentTo(v3));
}

TEST_CASE("Vertex incident edges")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

	auto edgeSet = getEdgeElements(v1.incidentEdges());
    REQUIRE(edgeSet.size() == 2);
    REQUIRE(edgeSet.count('m') == 1);
    REQUIRE(edgeSet.count('n') == 1);

	edgeSet = getEdgeElements(v2.incidentEdges());
    REQUIRE(edgeSet.size() == 1);
    REQUIRE(edgeSet.count('m') == 1);
}

TEST_CASE("Edge end vertices")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');

	auto e1Vertices = e1.endVertices();
    REQUIRE(*e1Vertices.first == 'a');
    REQUIRE(*e1Vertices.second == 'b');

	auto e2Vertices = e2.endVertices();
    REQUIRE(*e2Vertices.first == 'a');
    REQUIRE(*e2Vertices.second == 'c');
}

TEST_CASE("Edge opposite vertex")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(*e1.opposite(v1) == 'b');
    REQUIRE(*e1.opposite(v2) == 'a');
    REQUIRE(*e2.opposite(v1) == 'c');
    REQUIRE(*e2.opposite(v3) == 'a');
}

TEST_CASE("Edge is incident on")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto e1 = graph.insertEdge(v1, v2, 'm');
	auto e2 = graph.insertEdge(v1, v3, 'n');
    REQUIRE(e1.isIncidentOn(v1));
    REQUIRE(e1.isIncidentOn(v2));
    REQUIRE_FALSE(e1.isIncidentOn(v3));
    REQUIRE(e2.isIncidentOn(v1));
    REQUIRE(e2.isIncidentOn(v3));
    REQUIRE_FALSE(e2.isIncidentOn(v2));
}

TEST_CASE("Vertex iterator")
{
	Graph<int,char> graph = makeDefaultGraph();

	set<char> vertexSet;
	for (auto it = graph.vertices().begin(); it != graph.vertices().end(); ++it) {
        vertexSet.insert(**it);
	}
	REQUIRE(vertexSet.size() == 3);
	REQUIRE(vertexSet.contains('a'));
	REQUIRE(vertexSet.contains('b'));
	REQUIRE(vertexSet.contains('c'));

	vertexSet.clear();
	for (auto vertex : graph.vertices()) {
        vertexSet.insert(*vertex);
	}
	REQUIRE(vertexSet.size() == 3);
	REQUIRE(vertexSet.contains('a'));
	REQUIRE(vertexSet.contains('b'));
	REQUIRE(vertexSet.contains('c'));
}

TEST_CASE("Edge iterator")
{
	Graph<int,char> graph = makeDefaultGraph();

	set<char> edgeSet;
	for (auto it = graph.edges().begin(); it != graph.edges().end(); ++it) {
        edgeSet.insert(**it);
	}
	REQUIRE(edgeSet.size() == 2);
	REQUIRE(edgeSet.contains('m'));
	REQUIRE(edgeSet.contains('n'));

	edgeSet.clear();
	for (auto edge : graph.edges()) {
        edgeSet.insert(*edge);
	}
	REQUIRE(edgeSet.size() == 2);
	REQUIRE(edgeSet.contains('m'));
	REQUIRE(edgeSet.contains('n'));
}

TEST_CASE("Undirected edges")
{
	Graph<int,char> graph;
	auto& v1 = graph.insertVertex('a');
	auto& v2 = graph.insertVertex('b');
	auto& v3 = graph.insertVertex('c');
	auto& e1 = graph.insertEdge(v1, v2, 'm');
	auto& e2 = graph.insertEdge(v1, v3, 'n');
  	REQUIRE_FALSE(e1.isDirected());
  	REQUIRE_FALSE(e2.isDirected());
}

TEST_CASE("Directed edges")
{
	Graph<int,char> graph;
	auto& v1 = graph.insertVertex('a');
	auto& v2 = graph.insertVertex('b');
	auto& v3 = graph.insertVertex('c');
	auto& e1 = graph.insertDirectedEdge(v1, v2, 'm');
	auto& e2 = graph.insertDirectedEdge(v1, v3, 'n');
  	REQUIRE(e1.isDirected());
  	REQUIRE(e2.isDirected());
  	REQUIRE(*(e1.origin()) == 'a');
  	REQUIRE(*(e1.dest()) == 'b');
  	REQUIRE(*(e2.origin()) == 'a');
  	REQUIRE(*(e2.dest()) == 'c');
}

TEST_CASE("Out edges")
{
	Graph<int,char> graph;
	auto v1 = graph.insertVertex('a');
	auto v2 = graph.insertVertex('b');
	auto v3 = graph.insertVertex('c');
	auto& e1 = graph.insertDirectedEdge(v1, v2, 'm');
	auto& e2 = graph.insertDirectedEdge(v1, v3, 'n');

	auto edgeSet = getEdgeElements(v1.outEdges());
    REQUIRE(edgeSet.size() == 2);
    REQUIRE(edgeSet.count('m') == 1);
    REQUIRE(edgeSet.count('n') == 1);

	edgeSet = getEdgeElements(v2.outEdges());
    REQUIRE(edgeSet.size() == 0);

	edgeSet = getEdgeElements(v3.outEdges());
    REQUIRE(edgeSet.size() == 0);
}

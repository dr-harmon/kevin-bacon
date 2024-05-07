#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "bacon.h"

using namespace std;

void addRelationships(BaconFinder& baconFinder)
{
    baconFinder.addRelationship("Death of a Salesman", "John Malkovich");
    baconFinder.addRelationship("Death of a Salesman", "Dustin Hoffman");
    baconFinder.addRelationship("Sleepers", "Dustin Hoffman");
    baconFinder.addRelationship("Sleepers", "Kevin Bacon");
    baconFinder.addRelationship("Sleepers", "Brad Pitt");
    baconFinder.addRelationship("Fight Club", "Brad Pitt");
    baconFinder.addRelationship("Fight Club", "Edward Norton");
    baconFinder.addRelationship("Fight Club", "Helena Bonham Carter");
    baconFinder.addRelationship("The King's Speech", "Helena Bonham Carter");
    baconFinder.addRelationship("The King's Speech", "Colin Firth");
}

TEST_CASE("Bacon paths (short)")
{
	BaconFinder baconFinder("Kevin Bacon");
    addRelationships(baconFinder);

    REQUIRE(baconFinder.getBaconPath("Brad Pitt") == vector<string> {"Brad Pitt", "Sleepers", "Kevin Bacon"});
}

TEST_CASE("Bacon paths (long)")
{
	BaconFinder baconFinder("Kevin Bacon");
    addRelationships(baconFinder);

    REQUIRE(baconFinder.getBaconPath("Colin Firth") == vector<string> {"Colin Firth", "The King's Speech", "Helena Bonham Carter", "Fight Club", "Brad Pitt", "Sleepers", "Kevin Bacon"});
}

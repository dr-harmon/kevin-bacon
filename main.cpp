#include "bacon.h"
#include "csv.h"

#include <functional>
#include <iostream>
#include <string>

using namespace std;

void load(string filename, function<void(const string&,const string&)> callback)
{
    io::CSVReader<2, io::trim_chars<' '>, io::no_quote_escape<'\t'>> in(filename);
    string a, b;
    while (in.read_row(a, b)) {
		callback(a, b);
    }
}

int main()
{
	const string kevinBaconID = "nm0000102";
	BaconFinder baconFinder(kevinBaconID);

	unordered_map<string,string> names;
	load(NAMES_PATH, [&names](const string& id, const string& name) {
		names[id] = name;
	});

	unordered_map<string,string> titles;
	load(TITLES_PATH, [&titles](const string& id, const string& title) {
		titles[id] = title;
	});

	load(PRINCIPALS_PATH, [&baconFinder](const string& movie, const string& principal) {
		baconFinder.addRelationship(movie, principal);
	});

	const string actor = "nm0000163";  // Dustin Hoffman
	vector<string> path = baconFinder.getBaconPath(actor);

	transform(path.cbegin(), path.cend(), path.begin(), [&names,&titles](string str) {
		return str.rfind("nm", 0) == 0 ? names[str] : titles[str];
	});

	cout << "The Bacon number of " << names[actor] << " is " << (path.size() / 2) << ":" << endl << "  ";

	for (int i = 0; i < path.size(); i++) {
		cout << path[i];
		if (i < path.size() - 1) {
			cout << " -> ";
		}
	}
	cout << endl;

	return EXIT_SUCCESS;
}

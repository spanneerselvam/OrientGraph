#include <iostream>
#include <fstream>
#include "graph.h"

using namespace std;

int main(int argc, char *argv[])
{

    vector<string> names {"orient1.txt", "orient2.txt", "orient3.txt", "orient4.txt"};

    for (auto s: names)
    {
	    ifstream is(s);
	    if (is.fail())
	    {
		    cout << "Can't open file!" << endl;
		    is.close();
		    return 1;
	    }

	    graph G;
	    is >> G;
	    cout << "Input graph:" << endl;
	    cout << G << endl;

	    cout << "Oriented digraph: " << endl;
	    cout << G.orient() << endl;
	    is.close();
    }

    return 0;
}


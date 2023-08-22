// this programs checks if a given graph is eulerian, and if so, pronts the euler circle in it.
// please in put y/n if the graph is directed (y) or not (n)
// then enter number of vertixes, and number of arcs
// and then enter all the pairs of arcs (two vertixes)
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include "Vertex.h"
#include "Graphs.h"
using namespace std;

void read_input_and_update_graph(Graph& graph);

int main()
{
    Graph graph;
    read_input_and_update_graph(graph);
    graph.checkEulilerian();
    return 0;
}

// this function reads input from the user and create graph object
void read_input_and_update_graph(Graph& graph) {
    bool is_directed;
    int numOfVertices, numOfArcs;

    char c_is_directed;
    cout << "Is the graph directed: y/n" << endl;
    cin >> c_is_directed;
    if (c_is_directed != 'y' && c_is_directed != 'n')
    {
        cout << "invalid input";
        exit(1);
    }
    is_directed = (c_is_directed == 'y');

    cin >> numOfVertices;
    cin >> numOfArcs;
    if (numOfVertices < 1 || numOfArcs < 1)
    {
        cout << "invalid input";
        exit(1);
    }

    graph.updateData(is_directed, numOfVertices, numOfArcs);

    // read arcs and add them to graph
    for (int i = 0; i < numOfArcs; i++) {
        int from, to;
        cin >> from;
        cin >> to;
        graph.add_arc(from, to);
    }
}
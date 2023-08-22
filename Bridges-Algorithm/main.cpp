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
    graph.FindBridges();
    return 0;
}

// this function reads input from the user and create graph object
void read_input_and_update_graph(Graph& graph) {
    int numOfVertices, numOfArcs;

    cin >> numOfVertices;
    cin >> numOfArcs;
    if (numOfVertices < 1 || numOfArcs < 1)
    {
        cout << "invalid input";
        exit(1);
    }

    graph.updateData(false, numOfVertices, numOfArcs);

    // read arcs and add them to graph
    for (int i = 0; i < numOfArcs; i++) {
        int from, to;
        cin >> from;
        cin >> to;
        graph.add_arc(from, to);
    }
}
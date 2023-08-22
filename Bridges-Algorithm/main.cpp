//Student: Matan Hayon
//Id: 207334053


//this program gets undirected graph,
//it checks if the graph is connected - and if so the program
//finds all the bridges in the graph.
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
    if (numOfVertices < 0 || numOfArcs < 0)
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
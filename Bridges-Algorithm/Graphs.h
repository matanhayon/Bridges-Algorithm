#ifndef GRAPHS_H
#define GRAPHS_H

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <stack> 
#include "Vertex.h"
#include "Arc.h"

class Graph
{
private:
    bool is_directed;
    int num_of_vertices;
    int num_of_arcs;
    vector<list<Arc*>> neighborsList;
    vector<vector<bool>> arcsVisited;
    vector<Vertex*> vertexes;
    vector<int> parents;

public:
    Graph();

    bool getIsDirected() const { return is_directed; }
    int get_num_of_vertices() const { return num_of_vertices; }
    int get_num_of_arcs() const { return num_of_arcs; }
    list<Arc*> get_neighbors(int vertex) const { return neighborsList[vertex]; }

    void setIsDirected(bool i_isDirected) { is_directed = i_isDirected; }
    void searchBridgesVisit(Vertex* u, vector<int> GtParents, int* count);

    void add_arc(int from, int to);
    void checkArcValidity(int ver1, int ver2);
    void markOppositeDirectionArc(Vertex* neighbor_to_find, Vertex* vertex_to_update);
    bool isAllArcsMarked(Vertex* vertex, Arc** o_UnusedArcRes);
    void updateData(bool i_isDirected, int i_numOfVertices, int i_numOfArcs);
    void visit(Vertex* u, list<int>* endingList, Graph* tunedGraphResult);
    void MarkEdge(Vertex* from, Vertex* to, Graph* tunedGraphResult);
    void resetParentsArray();
    void createTransposeGraph(Graph* transposeGraph);
    bool isConnectedGraph();
    void resetVerticesColours();
    list<int> runDfsAndTuneArcsByFirstMove(Graph* undirectedGraph);
    void divideDirectedGraphToRakahim(list<int> dfsEndingList, Graph* transposeGraph);
    void searchArcsThatConnectsDifferentComponents(vector<int> GtParents);
    void FindBridges();
    ~Graph();
};

#endif // GRAPHS_H
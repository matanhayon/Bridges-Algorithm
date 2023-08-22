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

    void add_arc(int from, int to);
    void checkArcValidity(int ver1, int ver2);
    list<Vertex*> FindCircuit(Vertex* v0);
    void markOppositeDirectionArc(Vertex* neighbor_to_find, Vertex* vertex_to_update);
    bool isAllArcsMarked(Vertex* vertex, Arc** o_UnusedArcRes);
    void updateData(bool i_isDirected, int i_numOfVertices, int i_numOfArcs);
    void checkEulilerian();
    void findEuilerCircle();
    bool thereIsVertexInListWithUnusedEdges(list<Vertex*>& circle, Vertex** vertex);
    void printCircle(list<Vertex*> circle);
    void visit(Vertex* u, list<int>* endingList, Graph* tunedGraphResult);
    void MarkEdge(Vertex* from, Vertex* to, Graph* tunedGraphResult);
    void resetParentsArray();
    bool checkEuilerianForDirectedGraph();
    bool isStronglyConnectedGraph();
    void createTransposeGraph(Graph* transposeGraph);
    bool isInAndOutDegreeEqual();
    bool checkEuilerianForUndirectedGraph();
    bool areAllDegreesEven();
    bool isConnectedGraph();
    void resetVerticesColours();
    void FindBridges();
    list<int> runDfsAndTuneArcsByFirstMove(Graph* undirectedGraph);
    void divideDirectedGraphToRakahim(list<int> dfsEndingList, Graph* transposeGraph);
    void searchArcsThatConnectsDifferentComponents(vector<int> GtParents);
    ~Graph();
};

#endif // GRAPHS_H
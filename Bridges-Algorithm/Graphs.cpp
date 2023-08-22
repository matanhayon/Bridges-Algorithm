#include "Graphs.h"
#define NO_PARENT -1

// constructor
Graph::Graph()
{
    this->is_directed = false;
    this->num_of_vertices = 0;
    this->num_of_arcs = 0;
}

// this function adds arc to the graph, receives 2 vertices and connects them 
void Graph::add_arc(int from, int to)
{
    checkArcValidity(from, to);
    from--;
    to--;
    Vertex* vertex_from = vertexes[from];
    Vertex* vertex_to = vertexes[to];
    Arc* arc_to = new Arc(vertex_to);
    neighborsList[from].push_back(arc_to); // adds the vertice to the neighbours list

    if (!is_directed) // if the graph is undirected add the parallel arc
    {
        Arc* arc_from = new Arc(vertex_from);
        neighborsList[to].push_back(arc_from);
        vertex_from->set_degree(vertex_from->get_degree() + 1); // vertex from degree ++
        vertex_to->set_degree(vertex_to->get_degree() + 1); // vertex to degree ++
    }
    else
    {
        vertex_from->set_out_degree(vertex_from->get_out_degree() + 1);
        vertex_to->set_in_degree(vertex_to->get_in_degree() + 1);
    }
}

// check if the arc is correct
void Graph::checkArcValidity(int ver1, int ver2)
{
    if (ver1 > num_of_vertices || ver2 > num_of_vertices || ver1 < 1 || ver2 < 1)
    {
        cout << "invalid input";
        exit(1);
    }
}



// when the graph is not directed, mark the parallel arc
void Graph::markOppositeDirectionArc(Vertex* neighbor_to_find, Vertex* vertex_to_update)
{
    auto arcIt = neighborsList[vertex_to_update->get_id() - 1].begin();
    while (arcIt != neighborsList[vertex_to_update->get_id() - 1].end())
    {
        Arc* arcPtr = *arcIt;
        if (arcPtr->getVertex()->get_id() == neighbor_to_find->get_id())
        {
            arcPtr->setIsVisited(VISITED);
            break;
        }
        ++arcIt;
    }
}

// checks if all the arcs are marked
bool Graph::isAllArcsMarked(Vertex* vertex, Arc** o_UnusedArcRes)
{
    auto arcIt = neighborsList[vertex->get_id() - 1].begin();
    while (arcIt != neighborsList[vertex->get_id() - 1].end())
    {
        Arc* arcPtr = *arcIt;
        if (!arcPtr->getIsVisited())
        {
            *o_UnusedArcRes = arcPtr;
            return false;
        }
        ++arcIt;
    }
    return true;
}

// this function sets the data for the graph
void Graph::updateData(bool i_isDirected, int i_numOfVertices, int i_numOfArcs)
{
    this->is_directed = i_isDirected;
    this->num_of_vertices = i_numOfVertices;
    this->num_of_arcs = i_numOfArcs;
    this->neighborsList.clear();
    this->neighborsList.resize(num_of_vertices);
    this->vertexes.clear();
    this->vertexes.resize(num_of_vertices);
    this->parents.resize(num_of_vertices);

    for (int i = 0; i < num_of_vertices; i++)
    {
        Vertex* v = new Vertex(i + 1);
        this->vertexes[i] = v;
        this->parents[i] = NO_PARENT;
    }

    this->arcsVisited.resize(num_of_vertices);
    for (int i = 0; i < num_of_vertices; i++)
    {
        this->arcsVisited[i].resize(num_of_vertices);
        for (int j = 0; j < num_of_vertices; j++)
        {
            this->arcsVisited[i][j] = false;
        }
    }
}


// algorithm to go over the vertex's neighbours and marks them as visited
void Graph::visit(Vertex* u, list<int>* endingList, Graph* tunedGraphResult)
{
    Vertex* neighborVertex;
    int id = u->get_id();
    int neighborId;
    u->set_colour(Colour::GRAY);
    if (parents[id - 1] == NO_PARENT)
    {
        parents[id - 1] = id - 1;
    }

    for (Arc* neighborArc : neighborsList[id - 1])
    {
        neighborVertex = neighborArc->getVertex();
        neighborId = neighborVertex->get_id();

        if (!this->arcsVisited[id-1][neighborId-1] && !this->arcsVisited[neighborId-1][id-1])
        {
            MarkEdge(u, neighborVertex, tunedGraphResult);
            this->arcsVisited[id-1][neighborId-1] = true;
            this->arcsVisited[neighborId-1][id-1] = true;
        }

        if (neighborVertex->get_colour() == Colour::WHITE)
        {
            parents[neighborVertex->get_id() - 1] = parents[id - 1];
            visit(neighborVertex, endingList, tunedGraphResult);
        }
    }
    u->set_colour(Colour::BLACK);
    if (endingList != nullptr)
    {
        endingList->push_back(u->get_id());
    }
}

void Graph::MarkEdge(Vertex* from, Vertex* to, Graph* tunedGraphResult)
{
    if (tunedGraphResult != nullptr)
    {
        tunedGraphResult->add_arc(from->get_id(), to->get_id());
    }
}





// create the transpose graph
void Graph::createTransposeGraph(Graph* transposeGraph)
{
    transposeGraph->updateData(this->getIsDirected(), this->num_of_vertices, this->num_of_arcs);

    for (int i = 0; i < this->num_of_vertices; i++)
    {
        // for each vertex in the original graph, add all its outgoing edges in the reverse direction
        Vertex* v = this->vertexes[i];
        for (Arc* neighbor : neighborsList[i])
        {
            int neighbor_id = neighbor->getVertex()->get_id();
            transposeGraph->add_arc(neighbor_id, v->get_id());
        }
    }
}






// checks if a not directed graph is connected
bool Graph::isConnectedGraph()
{
    bool isConnected = true;
    visit(vertexes[0], nullptr, nullptr);
    for (int i = 0; isConnected && i < num_of_vertices; i++)
    {
        if (vertexes[i]->get_colour() != Colour::BLACK)
            isConnected = false;
    }

    return isConnected;
}

// sets all the colors to white
void Graph::resetVerticesColours()
{
    for (int i = 0; i < num_of_vertices; i++)
    {
        vertexes[i]->set_colour(Colour::WHITE);
        for (int j = 0; j < num_of_vertices; j++)
        {
            this->arcsVisited[i][j] = false;
        }
    }
}


//this is the main function: it finds the bridges on an undirected graph
void Graph::FindBridges()
{
    list<int> dfsEndingList;
    Graph tunedGraphResult;
    Graph transposeGraph;
    
    if (this->isConnectedGraph())
    {
        this->resetVerticesColours();
        this->resetParentsArray();
        tunedGraphResult.setIsDirected(true);
        dfsEndingList = runDfsAndTuneArcsByFirstMove(&tunedGraphResult);
        tunedGraphResult.divideDirectedGraphToRakahim(dfsEndingList, &transposeGraph);
        tunedGraphResult.searchArcsThatConnectsDifferentComponents(transposeGraph.parents);
    }
    else
    {
        cout << "Graph is not connected";
    }
}

void Graph::resetParentsArray()
{
    for (int i = 0; i < num_of_vertices; i++)
    {
        parents[i] = NO_PARENT;
    }
}

//gets undirected graph and tunes it to directed -
// the arrow will be in the direction of the first time moved in the arc
list<int> Graph::runDfsAndTuneArcsByFirstMove(Graph* tunedGraphResult)
{
    list<int> endingList;
    Vertex* startVertex = vertexes[0];

    this->resetVerticesColours();
    tunedGraphResult->updateData(true, this->num_of_vertices, this->num_of_arcs);

    for (Vertex* currentVertex : vertexes)
    {
        if (currentVertex->get_colour() == Colour::WHITE)
        {
            visit(currentVertex, &endingList, tunedGraphResult);
        }
    }

    return endingList;
}

//gets a transpose graph and ending list, does DFS with the reversed
//ending list as main loop. then divides the graph to rakahim (parents array)
void Graph::divideDirectedGraphToRakahim(list<int> dfsEndingList, Graph* transposeGraph)
{
    createTransposeGraph(transposeGraph);
    transposeGraph->resetVerticesColours();
    this->resetVerticesColours();
    
    list<int> reversedEndingList = dfsEndingList;
    reversedEndingList.reverse();

    for (list<int>::iterator it = reversedEndingList.begin(); it != reversedEndingList.end(); ++it)
    {
        int currentIndex = *it;
        Vertex* currentVertex = transposeGraph->vertexes[currentIndex - 1];
        if (currentVertex->get_colour() == Colour::WHITE)
        {
            transposeGraph->visit(currentVertex, nullptr, nullptr);
        }
    }
}



//this function gets a graph and the result of division to rakahim
//the function prints the arcs that connects different rakahim - the bridges.
void Graph::searchArcsThatConnectsDifferentComponents(vector<int> GtParents)
{
    int count = 0;
    this->resetVerticesColours();
    for (int i = 0; i < vertexes.size(); i++)
    {
        searchBridgesVisit(vertexes[i], GtParents, &count);
    }
    if (count == 0)
    {
        cout << "No bridges in graph";
    }
}

void Graph::searchBridgesVisit(Vertex* u, vector<int> GtParents, int* count)
{
    Vertex* neighborVertex;
    int id = u->get_id();
    int neighborId;
    u->set_colour(Colour::GRAY);

    for (Arc* neighborArc : neighborsList[id - 1])
    {
        neighborVertex = neighborArc->getVertex();
        neighborId = neighborVertex->get_id();

        if (neighborVertex->get_colour() == Colour::WHITE)
        {
            if (GtParents[neighborId - 1] != GtParents[id - 1])
            {
                cout << std::to_string(id) + " " + std::to_string(neighborId) << endl;
                (*count)++;
            }
            searchBridgesVisit(neighborVertex, GtParents, count);
        }
    }
    u->set_colour(Colour::BLACK);
}



// destructor
Graph::~Graph()
{
    for (Vertex* v : vertexes)
    {
        delete v;
    }

    for (list<Arc*> tempList : neighborsList)
    {
        for (Arc* tempArcPtr : tempList)
        {
            delete tempArcPtr;
        }
    }
}
#include "Graphs.h"

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

// this function finds a certain circle in a graph (receives a graph that has euler circle for sure)
list<Vertex*> Graph::FindCircuit(Vertex* v0)
{
    list<Vertex*> circuit;
    Vertex* v = v0;
    Vertex* u;
    Arc* unusedArc;
    circuit.push_back(v0);
    while (!this->isAllArcsMarked(v, &unusedArc))
    {
        u = unusedArc->getVertex();
        unusedArc->setIsVisited(VISITED);
        if (!is_directed)
            markOppositeDirectionArc(v, u);
        circuit.push_back(u);
        v = u;
    }
    return circuit;
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

    for (int i = 0; i < num_of_vertices; i++)
    {
        Vertex* v = new Vertex(i + 1);
        this->vertexes[i] = v;
    }
}

// checks if the graph is eulierean
void Graph::checkEulilerian()
{
    bool isEulerian;
    if (is_directed)
        isEulerian = this->checkEuilerianForDirectedGraph();
    else
        isEulerian = this->checkEuilerianForUndirectedGraph();

    if (isEulerian)
    {
        cout << "The graph is aulerian" << endl;
        this->findEuilerCircle();
    }
    else
        cout << "The graph is not aulerian" << endl;
}

// recevies a graph that has euler circle for sure and finds it, and prints the vertexes by the order he finds
void Graph::findEuilerCircle()
{
    list<Vertex*> circle = this->FindCircuit(vertexes[0]);
    Vertex* vertex;
    Arc* unusedArc;

    while (thereIsVertexInListWithUnusedEdges(circle, &vertex))
    {
        list<Vertex*> newCircle = FindCircuit(vertex);

        auto vertexIter = circle.begin();

        for (auto it = circle.begin(); it != circle.end(); ++it)
        {
            if (*it == vertex)
            {
                vertexIter = it;
                break;
            }
        }

        vertexIter--;
        newCircle.pop_back();
        if (vertexIter != circle.end())
        {
            circle.splice(next(vertexIter), newCircle);
        }
    }
    printCircle(circle);
}

// checks for the algorithm, if the list of the circle has a vertex with unused edges
bool Graph::thereIsVertexInListWithUnusedEdges(list<Vertex*>& circle, Vertex** vertex)
{
    auto arcIt = circle.begin();
    Arc* unusedArc; // dummy
    while (arcIt != circle.end())
    {
        if (!isAllArcsMarked(*arcIt, &unusedArc))
        {
            *vertex = *arcIt;
            return true;
        }
        ++arcIt;
    }
    return false;
}

// prints the circle in the graph
void Graph::printCircle(list<Vertex*> circle)
{
    string str_circle = "(";
    for (auto v : circle)
    {
        str_circle += to_string(v->get_id()); // convert int to string
        str_circle += ",";
    }
    str_circle[str_circle.length() - 1] = ')';

    cout << str_circle;
}

// algorithm to go over the vertex's neighbours and marks them as visited
void Graph::visit(Vertex* u)
{
    int id = u->get_id();
    u->set_colour(Colour::GRAY);

    for (Arc* neighbor : neighborsList[id - 1])
    {
        if (neighbor->getVertex()->get_colour() == Colour::WHITE)
        {
            visit(neighbor->getVertex());
        }
    }
    u->set_colour(Colour::BLACK);
}

// checks if a directed graph is eulerian
bool Graph::checkEuilerianForDirectedGraph()
{
    bool isEulerian;
    this->resetVerticesColours();
    if (this->isStronglyConnectedGraph() && this->isInAndOutDegreeEqual())
        isEulerian = true;
    else
        isEulerian = false;

    return isEulerian;
}

// checks if a directed graph is strongly connected
bool Graph::isStronglyConnectedGraph()
{
    if (!this->isConnectedGraph())
        return false;
    else
    {
        Graph transposeGraph;
        createTransposeGraph(transposeGraph);
        return transposeGraph.isConnectedGraph();
    }
}

// create the transpose graph
void Graph::createTransposeGraph(Graph& transposeGraph)
{
    transposeGraph.updateData(true, this->num_of_vertices, this->num_of_arcs);

    for (int i = 0; i < this->num_of_vertices; i++)
    {
        // for each vertex in the original graph, add all its outgoing edges in the reverse direction
        Vertex* v = this->vertexes[i];
        for (Arc* neighbor : neighborsList[i])
        {
            int neighbor_id = neighbor->getVertex()->get_id();
            transposeGraph.add_arc(neighbor_id, v->get_id());
        }
    }
}

// checks if all the in-degrees and out-degrees in the graph are equals
bool Graph::isInAndOutDegreeEqual()
{
    for (int i = 0; i < num_of_vertices; i++)
    {
        if (vertexes[i]->get_in_degree() != vertexes[i]->get_out_degree())
            return false;
    }
    return true;
}

// checks if an undirected graph is Euilerian, meaning the graph is connected, and all the degrees are even.
bool Graph::checkEuilerianForUndirectedGraph()
{
    bool isEulerian;
    this->resetVerticesColours();
    if (this->isConnectedGraph() && this->areAllDegreesEven())
        isEulerian = true;
    else
        isEulerian = false;

    return isEulerian;
}

// checks if all the degrees in the graph are even
bool Graph::areAllDegreesEven()
{
    for (int i = 0; i < num_of_vertices; i++)
    {
        if (vertexes[i]->get_degree() % 2 != 0) // checks if all the degrees are even
            return false;
    }

    return true;
}

// checks if a not directed graph is connected
bool Graph::isConnectedGraph()
{
    bool isConnected = true;
    visit(vertexes[0]);
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
    }
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
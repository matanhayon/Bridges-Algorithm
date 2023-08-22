#ifndef arc
#define arc
#define NOTVISITED false
#define VISITED true

#include "Vertex.h"
#include <iostream>
#include <vector>
#include <list>


class Arc
{
private:
    Vertex* vertex;
    bool isVisited;

public:
    Arc(Vertex* i_vertex = nullptr, bool i_isVisited = NOTVISITED)
    {
        vertex = i_vertex;
        isVisited = i_isVisited;
    }

    Vertex* getVertex() const { return vertex; }

    bool getIsVisited() const
    {
        return isVisited;
    }

    void setVertex(Vertex* i_vertex)
    {
        vertex = i_vertex;
    }

    void setIsVisited(bool i_isVisited)
    {
        isVisited = i_isVisited;
    }
};

#endif // !arc

#ifndef vertex_h
#define vertex_h

#include <iostream>
#include <list>
#include <vector>

using namespace std;

enum class Colour { WHITE, GRAY, BLACK };

class Vertex {
private:
    int id;
    Colour colour;
    int degree;
    int in_degree;
    int out_degree;

public:
    Vertex(int i_id = 0) {
        this->id = i_id;
        this->colour = Colour::WHITE;
        this->degree = 0;
        this->in_degree = 0;
        this->out_degree = 0;
    }


    int get_id() const {
        return id;
    }

    Colour get_colour() const {
        return colour;
    }

    void set_colour(Colour c) {
        colour = c;
    }

    void set_id(int i_id) {
        id = i_id;
    }

    int get_degree() const {
        return degree;
    }

    void set_degree(int d) {
        degree = d;
    }

    int get_in_degree() const {
        return in_degree;
    }

    void set_in_degree(int d) {
        in_degree = d;
    }

    int get_out_degree() const {
        return out_degree;
    }

    void set_out_degree(int d) {
        out_degree = d;
    }


};

#endif // !vertex_h
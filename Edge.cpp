#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
**************************************************************************************************/

// Constructor
Edge::Edge(int target_id){

    this->target_id = target_id;
    this->next_edge = nullptr;
    this->weight = 0;
    this->directed = false;
    this->marked = false;

}

// Destructor
Edge::~Edge(){


    //deleta todas as arestas??

    /*if (this->next_edge != nullptr){
        delete this->next_edge;
        this->next_edge = nullptr;
    }*/

    this->next_edge = nullptr;

}

// Getters

bool Edge::isMarked() const {
    return marked;
}

bool Edge::isDirected() const {
    return directed;
}

int Edge::getTargetId(){

    return this->target_id;

}

Edge* Edge::getNextEdge(){

    return this->next_edge;

}

float Edge::getWeight(){

    return this->weight;

}

// Setters

void Edge::setMarked(bool marked) {
    Edge::marked = marked;
}

void Edge::setDirected(bool directed) {
    Edge::directed = directed;
}

void Edge::setNextEdge(Edge* edge){

    this->next_edge = edge;

}

void Edge::setWeight(float weight){

    this->weight = weight;

}
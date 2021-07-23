/**************************************************************************************************
 * Implementation of the TAD Edge
**************************************************************************************************/

#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

using namespace std;

// Definition of the Edge class
class Edge{

    // Attributes
private:
    int target_id;
    Edge* next_edge;
    float weight;
    bool directed;

public:
    // Constructor
    Edge(int target_id);
    // Destructor
    ~Edge();
    // Getters
    int getTargetId();
    Edge* getNextEdge();
    float getWeight();
    // Setters
    void setNextEdge(Edge* edge);
    void setWeight(float weight);

    bool isDirected() const;

    void setDirected(bool directed);

};

#endif // EDGE_H_INCLUDED
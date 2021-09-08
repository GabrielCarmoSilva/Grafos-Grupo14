/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

//Atributes
private:
    int order;
    int number_edges;
    int total_groups;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node;
    Node* last_node;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    //Destructor
    ~Graph();
    //Getters
    int getOrder();
    int getNumberEdges();
    bool getDirected();
    bool getWeightedEdge();
    bool getWeightedNode();
    Node* getFirstNode();
    Node* getLastNode();
    //Other methods
    void insertNode(int id);
    void insertEdge(int id, int target_id, float weight);
    void removeNode(int id);
    bool searchNode(int id);
    Node* getNode(int id);

    //AGMG
    float auxPrimRandomizado(int initial_node, float alpha, int* parent, int* groups, int* node_groups);
    float auxPrimGuloso(int initial_node, int* parent, int* groups, int* node_groups, float* weights);
    float auxPrimReativo(int* node_groups, int* parent, int* best, float alpha, int max_iterations, int* doneiterations, double* weightFound);
    Graph* primReativoAGMG(float* alpha, int alpha_size, int iterations, int block);
    Graph* primRandomizadoAGMG(float alpha, int iterations);
    Graph* primGulosoAGMG();

    //meotodos statics e helpers
    Graph* ArrayToGraph(int* nodes, int total_nodes);
    float ArrayWeight(int* nodes, int total_nodes);
    int ArrayGroups(int* nodes, int total_nodes);
    bool nodeRange(int* parent, int* groups, int* node_groups, bool* nodes, float alpha, float* weights);
    int randEdge(int* parent, int* groups, int node);
    int randNode(bool* nodes);
    void insertNodeWithGroup(int id, int group);
    bool hasGroup(int groups[], int group);
    bool graphHasGroup(int group);
    bool addToGroup(int groups[], int group);
    int getTotalGroups() const;
    void setTotalGroups(int totalGroups);
    void markEdge(int id, int target_id);

    //imprimir e salvar
    void print();
    void save(ofstream& output_file);

};

#endif // GRAPH_H_INCLUDED
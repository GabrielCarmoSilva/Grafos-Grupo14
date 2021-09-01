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
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node;
    Node* last_node;

    //métodos auxiliares
    void auxFTI(int id, int visited[]);
    void auxFTD(int id, int visited[]);
    void auxBuscaEmProfundidade(int id, int visited[], Graph* retorno);
    bool auxaciclicoDirecionado(int id, int visited[], Graph* retorno, int order[]);
    int findMinimumEdge(int visited[], int nodes[], int total_nodes);

    //marcar arestas
    void markEdge(int id, int target_id);

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

    //methods phase1
    Graph* agmKruskal(int total_nodes, int nodes[]);
    Graph* agmPrim(int total_nodes, int nodes[]);
    Graph* floydMarshall(int idSource, int idTarget);
    Graph* dijkstra(int idSource, int idTarget);
    void directedTransitiveClosure(int id);
    int procuraMenorDistancia(float *dist, int *visitado, int NV);//
    Graph* FTI(int id);
    Graph* FTD(int id);
    Graph* BuscaEmProfundidade(int id);
    Graph* aciclicoDirecionado();

    //methods phase1
    //float greed();
    //float greedRandom();
    //float greedRactiveRandom();


    //AGMG
    void auxPrimAGMG(int initial_node, float alpha);

    //imprimir e salvar
    void print();
    void save(ofstream& output_file);

    //static e helpers
    bool isInList(int* list, int id);
    Graph* listToGraph(int nodeList[]);
    bool isInListPrim(int list[], int size, int id);
    Graph* listToGraphTree(int size, int nodeList[]);
    int getFromList(int list[], int size, int id);
    void insertNodeWithGroup(int id, int group);

    int getGroup() const;

    void setGroup(int group);

private:
    //Auxiliar methods

};

#endif // GRAPH_H_INCLUDED
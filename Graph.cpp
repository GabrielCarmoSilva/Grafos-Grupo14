#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = 0;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;

    //o readme disse para padronuizar em 0 a n-1
    //entretanto os arquivos nao estão nesse padrão e sim de 1 a n
    for(int i = 1; i <= order; i++)
        this->insertNode(i);

}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}


Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    if (!this->searchNode(id))
    {
        Node* new_node = new Node(id);

        if(this->first_node != nullptr)
        {
            this->last_node->setNextNode(new_node);
            this->last_node = new_node;
        }
        else
        {
            this->first_node = new_node;
            this->last_node = this->first_node;
        }

        this->order++;
    }
    else
    {
        cout << "ERROR: O no ja existe" << endl;
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    bool origin_node_check = this->searchNode(id);
    bool target_node_check = this->searchNode(target_id);
    if(origin_node_check && target_node_check)
    {
        Node* origin_node = this->getNode(id);
        if(!origin_node->searchEdge(target_id)){
            origin_node->insertEdge(this->getNode(target_id), this->directed, weight);
            //cout <<"aresta criada com no de origem " << id << " e no alvo " << target_id << endl;
        }
        else{
            cout << "ERROR: A aresta com no de origem" << id << " e no alvo " << target_id << " ja existe!" << endl;
        }
    }
    else
    {
        if(!origin_node_check && !target_node_check)
            cout << "ERROR: nenhum dos nos existe!" << endl;
        else if(!origin_node_check)
            cout << "ERROR: o no origem com id: <<" << id << " nao existe!" << endl;
        else
            cout << "ERROR: o no alvo com id: " << target_id << " nao existe!" << endl;
    }
}

void Graph::removeNode(int id){
    if(searchNode(id)) {
        Node* node = this->getFirstNode();
        Node* temp;
        if(node->getId() == id) {
            delete node;
        }
        else {
            while(node != nullptr) {
                if(node->getId() == id) {
                    temp->setNextNode(node->getNextNode());
                    break;
                }
                temp = node;
                node = node->getNextNode();
            }
            if(node == this->getLastNode()) {
                this->last_node = temp;
            }
            Node* aux = this->getFirstNode();
            while(aux != nullptr) {
                aux->removeEdge(node, this->directed);
                aux = aux->getNextNode();
            }
        }
        delete node;
        this->order--;
    }
    else {
        cout << "No nao encontrado!";
    }
}

bool Graph::searchNode(int id)
{
    if(this->first_node != nullptr)
    {
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if(aux->getId() == id)
            {
                return true;
            }
        }
    }
    return false;
}

Node *Graph::getNode(int id)
{
    if(this->first_node != nullptr)
    {
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            if(aux->getId() == id)
            {
                return aux;
            }
        }
        cout << "ERROR: No nao encontrado no grafo!" << endl;
        return NULL;
    }
    cout << "ERROR: O grafo esta vazio!" << endl;
    return NULL;
}

void Graph::print()
{
    if(this->first_node != nullptr)
    {
        for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
        {
            cout << "No: "  << aux->getId() << endl;
        }
    }
}

void Graph::save(ofstream& output_file){

    output_file << this->order << endl;
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
    {
        this->auxSave(aux, output_file);
    }

}


void Graph::auxSave(Node *node, ofstream& output_file){

    if(this->weighted_edge && !this->weighted_node){
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
            if(!aux->isDirected() || this->directed){
                output_file << node->getId()  << " " << aux->getTargetId() << " " << aux->getWeight() << endl;
            }
        }
    }

    if(!this->weighted_edge && !this->weighted_node){
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
            if(!aux->isDirected() || this->directed){
                output_file << node->getId()  << " " << aux->getTargetId() << endl;
            }
        }
    }

    if(this->weighted_edge && this->weighted_node){
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
            if(!aux->isDirected() || this->directed){
                output_file << node->getId() << " " << node->getWeight() << " " << aux->getTargetId() << " " ;
                output_file << this->getNode(aux->getTargetId())->getWeight() << " " << aux->getWeight() << endl;
            }
        }
    }

    if(!this->weighted_edge && this->weighted_node){
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
            if(!aux->isDirected() || this->directed){
                output_file << node->getId() << " " << node->getWeight() << " " << aux->getTargetId() << " " ;
                output_file << this->getNode(aux->getTargetId())->getWeight() <<  endl;
            }
        }
    }

}

//Function that prints a set of edges belongs breadth tree

void Graph::breadthFirstSearch(ofstream &output_file){

}



float Graph::floydMarshall(int idSource, int idTarget){

}


int Graph::procuraMenorDistancia(float *dist, int *visitado, int NV) {
    int i, menor = -1, primeiro = 1;
    for(i = 0; i < NV; i++) {
        if(dist[i] >= 0 && visitado[i] == 0) {
            if(primeiro) {
                menor = i;
                primeiro = 0;
            } else {
                if(dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void Graph::dijkstra(int idSource, int idTarget, ofstream& output_file) {
    int ant[this->order+1];
    float dist[this->order+1];
    int vizinhos[this->order+1];
    int i, cont, NV, ind, *visitado, u;
    cont = NV = this->order+1;
    visitado = (int*) malloc(NV * sizeof(int));
    for(i = 0; i < NV; i++) {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[idSource] = 0;
    while(cont > 0) {
        u = procuraMenorDistancia(dist, visitado, NV);
        if(u == -1)
            break;
        visitado[u] = 1;
        cont--;
        int j = 0;
        for(int k = 0; k < this->order+1; k++) {
            if(this->getNode(u)->searchEdge(k)) {
                vizinhos[j] = k;
                j++; 
            }
        }
        for(i = 0; i < this->getNode(u)->getOutDegree(); i++) {
            ind = vizinhos[i];
            if(dist[ind] < 0) {
                if(!this->weighted_edge) {
                    dist[ind] = dist[u] + 1;
                    ant[ind] = u;
                }
                else {
                    if(this->getNode(u)->searchEdge(ind)) {
                        dist[ind] = dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight();
                        ant[ind] = u;
                    }
                }    
            }
            else {
                if(!this->weighted_edge) {
                    if(dist[ind] > dist[u] + 1) {
                        dist[ind] = dist[u] + 1;
                        ant[ind] = u;
                    }
                }
                else {
                    if(this->getNode(u)->searchEdge(ind)) {
                        if(dist[ind] > dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight()) {
                            dist[ind] = dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight();
                            ant[ind] = u;
                        }
                    }
                }    
            }
        }
    }
    Graph* graph = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);
    while(idTarget != idSource) {
        if(!graph->searchNode(idTarget)) {
            graph->insertNode(idTarget);
        }
        if(!graph->searchNode(ant[idTarget])) {    
            graph->insertNode(ant[idTarget]);
        }
        graph->insertEdge(ant[idTarget], idTarget, this->getNode(ant[idTarget])->hasEdgeBetween(idTarget)->getWeight());
        idTarget = ant[idTarget];
    }
    graph->save(output_file);
}

//function that prints a topological sorting
void topologicalSorting(){

}

void breadthFirstSearch(ofstream& output_file){

}
Graph* getVertexInduced(int* listIdNodes){

}

Graph* agmKuskal() {
}

Graph* agmPrim(){

}

void Graph::directedTransitiveClosure(int id) {
    /*
    int foo[10000];
    Node* node = this->getNode(id);
    foo[0] = id;
    int i = 1;
    while(node->getFirstEdge() != nullptr) {
        Edge* edge = node->getFirstEdge();
        int id = edge->getTargetId();
        Node* targetNode = this->getNode(id);
        node->removeEdge(id, 0, targetNode);
        i++;
        foo[i] = id;
    }
    for(int j = 0; j < sizeof(foo); j++) {
        cout << foo[j];
    } */
}
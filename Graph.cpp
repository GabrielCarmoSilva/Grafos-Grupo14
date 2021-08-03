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
            this->number_edges++;
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



void Graph::dijkstra(int idSource, int idTarget, ofstream& output_file){
    
}

//function that prints a topological sorting
void topologicalSorting(){

}

void breadthFirstSearch(ofstream& output_file){

}
Graph* getVertexInduced(int* listIdNodes){

}

void Graph::agmKruskal(ofstream& output_file){
    Edge* S[this->getNumberEdges()] = {};
    Node* node = this->getFirstNode();
    Node* aux = node;
    Node* aux2 = nullptr;
    int i = 0;
    int index = 0;
    int flag_insert = 0;
    while(node != nullptr) {
        while(aux != nullptr) {
            if(node->searchEdge(aux->getId())) {
                S[i] = node->hasEdgeBetween(aux->getId());
                i++;
            }
            if(aux == this->last_node) {
                break;
            }
            aux = aux->getNextNode();
        }
        if(node == this->last_node) {
            break;
        }
        node = node->getNextNode();
        aux = node;
    }
    Node *node1 = this->getFirstNode();
    //aresta nao pode ter inicio e fim em elementos que ja estao no grafo
    Graph* graph = new Graph(0, 0, 1, 0);
    Edge* minWeightEdge = S[0];
    for(int k = 0; k < this->getNumberEdges(); k++) {
        for(int i = 0; i < this->getNumberEdges(); i++) {
            if(S[i] != nullptr && S[i]->getWeight() <= minWeightEdge->getWeight()) {
                minWeightEdge = S[i];
                index = i;
            }
        }
        S[index] = nullptr;
        if(graph->getFirstNode() != nullptr) {
            Node* aux2 = graph->getFirstNode();
        }
        flag_insert = false;
        while(node1 != nullptr) {
            cout << "Node 1: " << node1->getId() << endl;
            if(node1->searchEdge(minWeightEdge->getTargetId())) {
                if(aux2 == nullptr) {
                    cout << "Entrou nesse if" << endl;
                    flag_insert = 0;
                }
                else {
                    while(aux2 != nullptr) {
                        cout << "Aux2: " << aux2->getId() << endl;
                        cout << "Target Id: " << minWeightEdge->getTargetId() << endl;
                        if(aux2->getId() == minWeightEdge->getTargetId() || aux2->getId() == node1->getId()) {
                            flag_insert++;
                        }
                        aux2 = aux2->getNextNode();
                    }
                }
                if(flag_insert < 2) {
                    cout << "Entrou aqui para inserir os nós" << endl;
                    graph->insertNode(node1->getId());
                    graph->insertNode(minWeightEdge->getTargetId());
                    graph->insertEdge(node1->getId(), minWeightEdge->getTargetId(), this->directed);
                    cout << "Inseriu aresta do " << node1->getId() << " pro " << minWeightEdge->getTargetId() << endl;
                }
            }
            node1 = node1->getNextNode();
            if(graph->getFirstNode() != nullptr) {
                aux2 = graph->getFirstNode();
            }
            flag_insert = 0;
        }
        node1 = this->first_node;
        if(graph->getFirstNode() != nullptr) {
            aux2 = graph->getFirstNode();
        }
        flag_insert = 0;
    }
    graph->print();   
    graph->save(output_file);
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
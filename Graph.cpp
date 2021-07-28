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
    //entretanto os arquivos de exemplo nao estão nesse padrão e sim de 1 a n
    for(int i = 1; i <= order; i++)
        this->insertNode(i);

}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    //destruindo a lista de nós e suas arestas
    while (next_node != nullptr)
    {
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
    //checagem para ver se ambos os nos existem e a aresta pode ser criada
    if(origin_node_check && target_node_check)
    {
        Node* origin_node = this->getNode(id);
        if(!origin_node->searchEdge(target_id)){
            origin_node->insertEdge(this->getNode(target_id), this->directed, weight);
        }
        else{
            cout << "ERROR: A aresta com no de origem" << id << " e no alvo " << target_id << " ja existe!" << endl;
        }
    }
    else
    {
        //tratamento do erro, caso nao existe um dos nos
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
        Node* temp = nullptr;


        //busca pelo no desejado e do temp
        // para manter a integridade da lista
        while(node->getId() != id) {
            temp = node;
            node = node->getNextNode();
        }

        if(temp != nullptr)
            temp->setNextNode(node->getNextNode());
        else
            this->first_node = node->getNextNode();

        if(node == this->last_node)
            this->last_node = temp;

        if(node->getNextNode() == this->last_node)
            this->last_node = node->getNextNode();

        Node* aux = this->getFirstNode();

        //removendo arestas ligadas a ele mas que não partem desse nó
        while(aux != nullptr) {
            aux->removeEdge(node, this->directed);
            aux = aux->getNextNode();
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
        return nullptr;
    }
    cout << "ERROR: O grafo esta vazio!" << endl;
    return nullptr;
}

void Graph::print()
{
    //impressão de todas as arestas do grafo
    if(this->first_node != nullptr)
    {
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode()){
            cout << "No: " << node->getId() << endl;
            for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
                if(!aux->isDirected() || this->directed){
                    cout << node->getId()  << " " << aux->getTargetId() << endl;
                }
            }
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



float Graph::dijkstra(int idSource, int idTarget){

}

//function that prints a topological sorting
void topologicalSorting(){

}

void breadthFirstSearch(ofstream& output_file){

}
Graph* getVertexInduced(int* listIdNodes){

}

Graph* agmKuskal(){

}

Graph* agmPrim(){

}

//FECHO TRANSITIVO DIRETO
Graph* Graph::FTD(int id) {
    if(this->directed){

        //preenchimento da lista de nos visitados
        int visited[this->order+1];
        for(int i = 1; i <= this->order; i++)
            visited[i] = -1;

        visited[id] = id;

        auxFTD(id, visited);

        cout << "imprimindo nos do fecho transitivo direto!" << endl;
        for(int k = 1; k <= this->order; k++){
            if(visited[k] != -1)
                cout << visited[k] << endl;
        }
        return nullptr;

    } else{
        cout << "ERROR: O Grafo deve ser direcionado!" << endl;
    }

}

//FECHO TRANSITIVO DIRETO AUXILIAR
void Graph::auxFTD(int id, int visited[]) {
    Node* node = this->getNode(id);
    if(node != nullptr){
        //busca em profundiade para encontrar todos os caminhos possíveis pelo nó
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
            if(visited[aux->getTargetId()] == -1){
                visited[aux->getTargetId()] = aux->getTargetId();
                auxFTD(aux->getTargetId(), visited);

            }
        }
    }
}

//FECHO TRANSITIVO INDIRETO
Graph* Graph::FTI(int id) {
    if(this->directed){

        //preenchimento da lista de nos visitados
        int visited[this->order+1];
        for(int i = 1; i <= this->order; i++)
            visited[i] = -1;

        visited[id] = id;

        auxFTI(id, visited);

        cout << "imprimindo nos do fecho transitivo indireto!" << endl;
        for(int k = 1; k <= this->order; k++){
            //impressão de todos os nós visitados que
            //formas o fecho transitivo indireto excluindo o no de partida
            if(visited[k] != -1 && k != id)
                cout << visited[k] << endl;
        }
        return nullptr;

    } else{
        cout << "ERROR: O Grafo deve ser direcionado!" << endl;
    }
}

//FECHO TRANSITIVO INDIRETO AUXILIAR
void Graph::auxFTI(int id, int visited[]) {
    //busca por qualquer no que possua aresta apontada para o id atual
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode()){
        if(visited[aux->getId()] == -1 && aux->searchEdge(id)){
            visited[aux->getId()] = aux->getId();
            auxFTI(aux->getId(), visited);
        }
    }
}

//BUSCA EM PROFUNDIADE DESTACANDO ARESTAS DE RETORNO
Graph* Graph::BuscaEmProfundidade(int id){

    //criação e preenchimento das lista de nos visitados e o grafo
    //para armazenar as arestas de retorno
    int visited[this->order+1];
    Graph *retorno = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);

    for(int i = 1; i <= this->order; i++){
        visited[i] = -1;
    }

    visited[id] = 0;





    //Possíveis valores no vetor visited, que representa os nos do grafo
    // -1 = no não visitado
    //  numero > 0 (id do no anterior) = visitado 1 vez
    //  -2 = no que todas as arestas já foram visitadas
    auxBuscaEmProfundidade(id, visited, retorno);

    //imprimindo no console caminho realizado
    cout << "Arvore gerada pela busca em profundidade" << endl;
    for(int k = 1; k <= this->order; k++){
        if(visited[k] != -1){
            cout << "No: " << k << endl;
        }
    }

    //imprimindo as areas de retorno no padrão "Nó origem Nó alvo"
    cout << "Arestas de retorno: " << endl;
    retorno->print();
    return retorno;
}

//BUSCA EM PROFUNDIADE DESTACANDO ARESTAS DE RETORNO AUXILIAR
void Graph::auxBuscaEmProfundidade(int id, int visited[], Graph* retorno){
    Node* node = this->getNode(id);
    if(node != nullptr){
        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){

            if(visited[aux->getTargetId()] == -1){

                //caso seja um grafo nao direcionado
                //desconsiderar aresta de volta
                if(!this->directed){
                    visited[node->getId()] = aux->getTargetId();
                }

                visited[aux->getTargetId()] = node->getId();

                this->auxBuscaEmProfundidade(aux->getTargetId(), visited, retorno);

            } else if(visited[aux->getTargetId()] >= 0 && visited[aux->getTargetId()] != node->getId() ){

                //preenchendo grafo com arestas de retorno
                if(!retorno->searchNode(node->getId())){
                    retorno->insertNode(node->getId());
                }

                if(!retorno->searchNode(aux->getTargetId())){
                    retorno->insertNode(aux->getTargetId());
                }

                retorno->insertEdge(node->getId(), aux->getTargetId(), 0);
            }
        }
        visited[node->getId()] = -2;
    }
}
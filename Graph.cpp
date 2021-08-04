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

    //preenchendo grafo com os nos
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


        // busca pelo no desejado e do temp
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

        // removendo arestas ligadas a ele mas que não partem desse nó
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

//printar todos os nós com suas respectivas arestas no console
void Graph::print()
{
    // impressão de todas as arestas do grafo
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

//função para salvar um grafo no arquivo de output na linguagem dot
void Graph::save(ofstream& output_file){

    //definindo tipo de grafo para o dot
    string graphType = this->directed ? "strict digraph {" : "strict graph {";
    output_file << graphType << endl;
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode()){
        output_file << aux->getId();
        if(this->weighted_node){
            output_file << " [weight=\"" << aux->getWeight() << "\"]";
        }
        output_file << ";" << endl;
    }

    if(this->directed){
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode()){
            for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                output_file << node->getId() << " -> " << edge->getTargetId();
                if(this->weighted_edge) {
                    output_file << " " << "[label=\"" << edge->getWeight() << "\",weight=\"" << edge->getWeight() << "\"]";
                }
                output_file << ";" << endl;
            }
        }
    } else{
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode()){
            for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                if(!edge->isDirected()){
                    output_file << node->getId() << " -- " << edge->getTargetId();
                    if(this->weighted_edge){
                        output_file << " " << "[label=\"" << edge->getWeight() << "\",weight=\"" << edge->getWeight() << "\"]";
                    }
                    output_file << ";" << endl;
                }
            }
        }
    }
    output_file << "}" << endl;
}


//função que transormará uma lista de nos visitados no grafo resultante baseado no grafo existente
Graph* Graph::listToGraph(int nodeList[]){
    //grafo que será gerado
    Graph* result = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);

    for(Node *node = this->first_node; node != nullptr; node = node->getNextNode()){

        //verificando se o nó está na lista
        if(this->isInList(nodeList, node->getId())){

            //verificando se o nó existe no grafo resultante
            if(!result->searchNode(node->getId())){
                result->insertNode(node->getId());
            }

            for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){

                //verificando se o nó alvo existe na lista
                if(this->isInList(nodeList,  edge->getTargetId())){

                    //verificando se o nó alvo existe no grafo resultante
                    if(!result->searchNode(edge->getTargetId())){
                        result->insertNode(edge->getTargetId());
                    }

                    //inserindo aresta ao grafo resultante
                    result->insertEdge(node->getId(), edge->getTargetId(), edge->getWeight());
                }
            }
        }
    }
    return result;
}


//função para verificar a existencia de um nó em uma lista de nos visitados
bool Graph::isInList(int* list, int id){
    for(int i = 1; i <= this->order; i++){
        if(i == id && list[i] != -1){
            return true;
        }
    }
    return false;
}

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
    if(idTarget == -1 || ant[idTarget] == -1) {
        cout << "Caminho não encontrado!" << endl;
    }
    else {
        while(idTarget != idSource && idTarget != -1 && ant[idTarget] != -1) {
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

//FECHO TRANSITIVO DIRETO
Graph* Graph::FTD(int id) {
    if(this->directed){

        //preenchimento da lista de nos visitados
        int visited[this->order+1];
        for(int i = 1; i <= this->order; i++)
            visited[i] = -1;

        auxFTD(id, visited);

        cout << "imprimindo nos do fecho transitivo direto!" << endl;
        for(int k = 1; k <= this->order; k++){
            if(visited[k] != -1)
                cout << visited[k] << endl;
        }

        return this->listToGraph(visited);

    } else{
        cout << "ERROR: O Grafo deve ser direcionado!" << endl;
        return nullptr;
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

        auxFTI(id, visited);

        cout << "imprimindo nos do fecho transitivo indireto!" << endl;
        for(int k = 1; k <= this->order; k++){
            if(visited[k] != -1)
                cout << visited[k] << endl;
        }
        return this->listToGraph(visited);;

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
    Graph *resultado = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);

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


                //adicionando nós ao grafo de retonro
                if(!retorno->searchNode(node->getId())){
                    retorno->insertNode(node->getId());
                }

                if(!retorno->searchNode(aux->getTargetId())){
                    retorno->insertNode(aux->getTargetId());
                }

                retorno->insertEdge(node->getId(), aux->getTargetId(), aux->getWeight());

                this->auxBuscaEmProfundidade(aux->getTargetId(), visited, retorno);

            } else if(visited[aux->getTargetId()] >= 0 && visited[aux->getTargetId()] != node->getId() ){

                //adicionando nós ao grafo de retonro caso nao existam

                //preenchendo grafo com as arestas de retorno
                retorno->insertEdge(node->getId(), aux->getTargetId(), 0);
            }
        }
        visited[node->getId()] = -2;
    }
}
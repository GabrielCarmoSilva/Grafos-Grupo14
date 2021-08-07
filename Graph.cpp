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
            this->number_edges++;
            //cout <<"aresta criada com no de origem " << id << " e no alvo " << target_id << endl;
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

void Graph::markEdge(int id, int target_id)
{
    if(searchNode(id) && searchNode(target_id)){
        Edge* edge = this->getNode(id)->hasEdgeBetween(target_id);
        if(edge != nullptr){
            edge->setMarked(true);
        } else{
            cout << "ERROR: ARESTA NÃO EXISTE!" << endl;
        }
    } else if(!searchNode(id)){
        cout << "ERROR: NO DE ORIGEM NAO EXISTE!" << endl;
    } else{
        cout << "ERROR: NO ALVO NAO EXISTE!" << endl;
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
        cout << "Imprimindo Grafo: " << endl;
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode()){
            cout << endl << "No: " << node->getId() << endl;
            cout << "Arestas do No: " << endl;
            int i = 1;
            int j = 1;
            for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){
                if(aux->isMarked()){
                    cout << "Aresta de RETORNO " << j << ": ";
                    j++;
                } else{
                    cout << "Aresta " << i << ": ";
                    i++;
                }
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

    //imprimindo todos os nós
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode()){
        output_file << aux->getId();
        if(this->weighted_node){
            output_file << " [weight=\"" << aux->getWeight() << "\"]";
        }
        output_file << ";" << endl;
    }

    //imprimindo todas as arestas
    if(this->directed){
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode()){
            for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                output_file << node->getId() << " -> " << edge->getTargetId();

                if(this->weighted_edge) {
                    output_file << " " << "[label=\"" << edge->getWeight() << "\",weight=\"" << edge->getWeight() << "\"]";
                }

                if(edge->isMarked()){
                    output_file <<  "[color=\"red\"][penwidth = 2]";
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

                    if(edge->isMarked()){
                        output_file <<  "[color=\"red\"][penwidth = 2]";
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


float Graph::floydMarshall(int idSource, int idTarget){

}


int Graph::procuraMenorDistancia(float *dist, int *visitado, int NV) {
    //procura o vertice com a menor distancia para o vertice de origem 
    int i, menor = -1, primeiro = 1;
    for(i = 0; i < NV; i++) {
        if(dist[i] >= 0 && visitado[i] == 0) { //verifica se a distancia realmente existe e se o vertice ainda nao foi visitado pelo caminho minimo
            if(primeiro) { //verifica se foi visitado pela primeira vez
                menor = i; //recebe vertice
                primeiro = 0;
            } else {
                if(dist[menor] > dist[i]) //verifica se a distancia do menor achado é maior do que a distancia para o vertice origem da iteração atual
                    menor = i;
            }
        }
    }
    return menor;
}

Graph* Graph::dijkstra(int idSource, int idTarget) {
    int ant[this->order+1]; // armazena o vertice anterior de cada indice no caminho minimo
    float dist[this->order+1]; //armazena a distancia de cada vertice ao vertice inicial
    int vizinhos[this->order+1]; //armazena todas os nos do vertice nos quais possuem arestas
    int i, cont, NV, ind, *visitado, u;
    cont = NV = this->order+1; //achar numero de vertices do grafo
    visitado = (int*) malloc(NV * sizeof(int)); //aloca dinamicamente vetor com numero de vertices e verifica se ja foi visitado
    for(i = 0; i < NV; i++) { //inicializando vetores
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[idSource] = 0; //distancia do idSource pra ele mesmo é 0
    while(cont > 0) {
        u = procuraMenorDistancia(dist, visitado, NV); //procura o vertice com menor distancia para o vertice de origem
        if(u == -1)
            break;
        visitado[u] = 1; //indica que o vertice u ja foi visitado pelo caminho
        cont--; //subtrai o vertice ja visitado da contagem
        int j = 0;
        for(int k = 0; k < this->order+1; k++) {
            if(this->getNode(u)->searchEdge(k)) {
                vizinhos[j] = k; //acha todos os vertices vizinhos de u
                j++; 
            }
        }
        for(i = 0; i < this->getNode(u)->getOutDegree(); i++) {
            ind = vizinhos[i]; //vertice que possui aresta em u
            if(dist[ind] < 0) { //se a distancia de ind é -1, ou seja, se ainda não foi calculada a distancia desse vertice para o vertice inicial
                if(!this->weighted_edge) {
                    dist[ind] = dist[u] + 1; //se a aresta nao tem peso, soma mais 1 na distancia
                    ant[ind] = u; //o anterior de ind no caminho é u
                }
                else {
                    if(this->getNode(u)->searchEdge(ind)) {
                        dist[ind] = dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight(); //aresta tem peso, entao soma o peso da aresta
                        ant[ind] = u; //o anterior de ind no caminho é u
                    }
                }    
            }
            else { //se a distancia desse vertice para o vertice inicial ja foi calculada
                if(!this->weighted_edge) {
                    if(dist[ind] > dist[u] + 1) { //se a distancia ja setada para esse vertice é maior do que a distancia calculada agora
                        dist[ind] = dist[u] + 1; //se a aresta nao tem peso, soma mais 1 na distancia
                        ant[ind] = u; //o anterior de ind no caminho é u
                    }
                }
                else {
                    if(this->getNode(u)->searchEdge(ind)) {
                        if(dist[ind] > dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight()) {
                            dist[ind] = dist[u] + this->getNode(u)->hasEdgeBetween(ind)->getWeight(); //aresta tem peso, entao soma o peso da aresta
                            ant[ind] = u; //o anterior de ind no caminho é u
                        }
                    }
                }    
            }
        }
    }
    Graph* graph = new Graph(0, this->directed, this->weighted_edge, this->weighted_node); //cria o grafo que ira armazenar o caminho minimo
    if(idTarget == -1 || ant[idTarget] == -1) { //se nao tem caminho, o anterior nunca deixou de ser -1
        cout << "Caminho não encontrado!" << endl;
    }
    else {
        //todos os vertices do caminho estao no vetor ant. exemplo: se o grafo tem aresta de 1 para 2, o ant de 2 é 1. logo, o idSource será a última posição setada do vetor ant, e o idTarget, a primeira.
        while(idTarget != idSource && idTarget != -1 && ant[idTarget] != -1) {
            if(!graph->searchNode(idTarget)) { //verifica se nó já foi inserido
                graph->insertNode(idTarget); //insere nó
            }
            if(!graph->searchNode(ant[idTarget])) {  //verifica se nó já foi inserido  
                graph->insertNode(ant[idTarget]); //insere o nó anterior
            }
            graph->insertEdge(ant[idTarget], idTarget, this->getNode(ant[idTarget])->hasEdgeBetween(idTarget)->getWeight()); //insere aresta entre os dois nós que foram inseridos com o peso do grafo original
            idTarget = ant[idTarget]; //recebe o vértice anterior a ele mesmo
        }
    }
    return graph;
}      
    //function that prints a topological sorting
void Graph::topologicalSorting(){
}

//function that prints a topological sorting
void topologicalSorting(){

}

Graph* Graph::getVertexInduced(int* listIdNodes){

}

Graph* Graph::agmKruskal(){
    int i, j, dest, primeiro, NV = this->order, cont = 0;
    int pai[order+1]; //vetor para setar o pai de cada vertice na arvore
    int vizinhos[this->order]; //vetor para encontrar todos os vertices que possuem arestas em um vertice especifico
    double menorPeso;
    int orig = this->getFirstNode()->getId();
    int *arv = (int*) malloc(NV*sizeof(int)); //vetor para encontrar a arvore de cada vertice
    for(i = 1; i <= NV; i++) { //inicializando vetores
        arv[i] = i;
        pai[i] = -1;
        vizinhos[i] = -1;
    }
    pai[orig] = orig; //seta o pai de origem como a própria origem
    while(1) { //loop infinito até ser quebrado
        primeiro = 1;
        for(i = 1; i <= NV; i++) {
            for(int k = 1; k <= NV; k++) {
                if(this->getNode(i)->searchEdge(k)) {
                    vizinhos[cont] = k; //seta todos os vertices vizinhos de i
                    cont++; 
                }
            }
            cont = 0; //zera a variavel para o proximo vertice
            for(j = 0; j < this->getNode(i)->getOutDegree(); j++) { //analisa todas as arestas do vertice
                if(arv[i] != arv[vizinhos[j]]) { //se a arvore de i e de seus vizinhos sao diferentes, caso seja, o vertice vizinho pode ser inserido
                    if(primeiro) { //verifica se é a primeira vez que está visitando esse vértice
                        menorPeso = this->getNode(i)->hasEdgeBetween(vizinhos[j])->getWeight(); //seta o peso da aresta de i para seu vizinho como a menor
                        orig = i;
                        dest = vizinhos[j];
                        primeiro = 0;
                    }
                    else { //caso nao seja a primeira vez que esse vertice esta visitado
                        if(menorPeso > this->getNode(i)->hasEdgeBetween(vizinhos[j])->getWeight()) { //se o peso atual é maior do que o peso da aresta de i para seu vizinho em questão
                            menorPeso = this->getNode(i)->hasEdgeBetween(vizinhos[j])->getWeight(); //seta o menor peso como o peso da aresta de i para seu vizinho
                            orig = i;
                            dest = vizinhos[j];
                        } 
                    }
                }
            }
            for(int t = 1; t <= NV; t++) {
                vizinhos[t] = -1; //inicializa o vetor como -1 para o proximo vertice
            }
        }
        if(primeiro == 1) break; //achou a arvore minima, entao sai do loop
        if(pai[orig] == -1) pai[orig] = dest; //se o pai de tal vertice é -1, significa que ele não tem um vertice pai, entao setando como ele mesmo
        else pai[dest] = orig; //setando o pai do vertice caso ele tenha

        for(i = 1; i <= NV; i++) {
            if(arv[i] == arv[dest]) {
                arv[i] = arv[orig]; //colocando todos os vertices achados na mesma arvore
            }
        }
    }
    Graph* graph = new Graph(0, this->directed, this->weighted_edge, this->weighted_node); //criando grafo
    for(int i = 1; i <= NV; i++) {
        if(pai[i] != i) { //verifica se o pai de i e i não são iguais, porque nesse caso teriamos um self-loop, o que nao é o caso
            if(!graph->searchNode(i)) {
                graph->insertNode(i); //insere i
            }
            if(!graph->searchNode(pai[i]) && pai[i] != -1) {
                graph->insertNode(pai[i]); //insere pai de i
            }
            if(graph->searchNode(pai[i]) && graph->searchNode(i) && pai[i] != -1) {
                if(!this->directed) { //verifica se aresta nao e direcionada, entao nao existe preocupacao com a ordem dos vertices na aresta 
                    graph->insertEdge(i, pai[i], this->getNode(i)->hasEdgeBetween(pai[i])->getWeight());
                }
                else {
                    if(this->getNode(i)->searchEdge(pai[i])) { //caso a aresta seja direcionada, preciso saber se a aresta vai de i pra pai ou de pai pra i
                        graph->insertEdge(i, pai[i], this->getNode(i)->hasEdgeBetween(pai[i])->getWeight()); //insere aresta de forma correta
                    }
                    else if(this->getNode(pai[i])->searchEdge(i)) {
                        graph->insertEdge(pai[i], i, this->getNode(pai[i])->hasEdgeBetween(i)->getWeight());
                    }
                }
            }
        }    
    }
    return graph;
}

Graph* Graph::agmPrim(){

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


    //imprimindo as arestas de retorno eo próprio grafo gerado
    cout << "Arvore gerada pela busca em profundidade" << endl;
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

            } else if(visited[aux->getTargetId()] >= 0 && visited[node->getId()] >= 0 && (this->directed || visited[aux->getTargetId()] != node->getId()) ){
                //preenchendo grafo com as arestas de retorno
                retorno->insertEdge(node->getId(), aux->getTargetId(), 0);
                retorno->markEdge(node->getId(), aux->getTargetId());

            }
        }
        visited[node->getId()] = -2;
    }
}
Graph* Graph::aciclicoDirecionado(){

    if(this->directed){
        //criação e preenchimento das lista de nos visitados e o grafo
        //para armazenar as arestas de retorno
        int visited[this->order+1];
        int order[this->order+1];
        Graph *retorno = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);

        for(int i = 1; i <= this->order; i++){
            visited[i] = -1;
            order[i] = -1;
        }

        //Possíveis valores no vetor visited, que representa os nos do grafo
        // -1 = no não visitado
        //  numero >= 0 (id do no anterior) = visitado 1 vez
        //  -2 = no que todas as arestas já foram visitadas
        for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
            if(visited[node->getId()] == -1){
                if(!auxaciclicoDirecionado(node->getId(), visited, retorno, order)){
                    cout << "Grafo nao eh aciclico!" << endl;
                    return nullptr;
                }
            }
        }

        //imprimindo no console ordem topologica
        cout << "Ordem topológica gerada pela busca em profundidade" << endl;
        for(int k = 1; k <= this->order; k++){
            if(order[k] != -1){
                cout << "No: " << order[k] << endl;
            }
        }

        return retorno;
    } else{

        cout << "O grafo deve ser direcionado!" << endl;
        return nullptr;
    }

}
bool Graph::auxaciclicoDirecionado(int id, int visited[], Graph* retorno, int order[]){
    Node* node = this->getNode(id);
    if(node != nullptr){
        bool condition = true;
        //adicionando nós ao grafo de retonro
        if(!retorno->searchNode(node->getId())){
            retorno->insertNode(node->getId());
        }

        for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge()){


            if(visited[aux->getTargetId()] == -1){

                visited[node->getId()] = node->getId();
                visited[aux->getTargetId()] = node->getId();

                if(!retorno->searchNode(aux->getTargetId())){
                    retorno->insertNode(aux->getTargetId());
                }

                condition = this->auxaciclicoDirecionado(aux->getTargetId(), visited, retorno, order);

                retorno->insertEdge(node->getId(), aux->getTargetId(), aux->getWeight());

            } else if(visited[aux->getTargetId()] >= 0 && visited[node->getId()] >= 0){

                return false;
            }
        }
        visited[node->getId()] = -2;
        for(int i = this->order; i > 0; i--){
            if(order[i] == -1){
                order[i] = node->getId();
                break;
            }
        }
        return condition;
    }
    return true;

}
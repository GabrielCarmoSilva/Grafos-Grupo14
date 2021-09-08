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
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
    this->total_groups = 0;

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
//Funcao que verifica se o grafo eh direcionado
bool Graph::getDirected()
{
    return this->directed;
}
//Funcao que verifica se o grafo tem peso nas arestas
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}
//Funcao que verifica se o grafo tem peso nos nos
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}
//Funcao que retorna o primeiro no
Node *Graph::getFirstNode()
{
    return this->first_node;
}
//Funcao que retorna o ultimo no
Node *Graph::getLastNode()
{
    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
//Funcao que insere no
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

void Graph::insertNodeWithGroup(int id, int group)
{
    if (!this->searchNode(id))
    {
        Node* new_node = new Node(id);
        new_node->setGroup(group);

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

//Funcao que insere aresta
void Graph::insertEdge(int id, int target_id, float weight)
{
    bool origin_node_check = this->searchNode(id);
    bool target_node_check = this->searchNode(target_id);
    //checagem para ver se ambos os nos existem e a aresta pode ser criada
    if(origin_node_check && target_node_check)
    {
        Node* origin_node = this->getNode(id);
        if(!origin_node->searchEdge(target_id))
        {
            origin_node->insertEdge(this->getNode(target_id), this->directed, weight);
            this->number_edges++;
        }
        else
        {
            cout << "ERROR: A aresta com no de origem " << id << " e no alvo " << target_id << " ja existe!" << endl;
        }
    }
    else
    {
        //tratamento do erro, caso nao existe um dos nos
        if(!origin_node_check && !target_node_check)
            cout << "ERROR: nenhum dos nos existe!" << endl;
        else if(!origin_node_check)
            cout << "ERROR: O no origem com id: " << id << " nao existe!" << endl;
        else
            cout << "ERROR: O no alvo com id: " << target_id << " nao existe!" << endl;
    }
}
//Funcao que marca aresta de retorno
void Graph::markEdge(int id, int target_id)
{
    if(searchNode(id) && searchNode(target_id))
    {
        Edge* edge = this->getNode(id)->hasEdgeBetween(target_id);
        if(edge != nullptr)
        {
            edge->setMarked(true);
        } else
            {
                cout << "ERROR: Aresta nao existe!" << endl;
            }
    } else if(!searchNode(id))
            {
                cout << "ERROR: No de origem nao existe!" << endl;
            } else
                {
                    cout << "ERROR: No alvo nao existe!" << endl;
                }
}
//Funcao que remove no
void Graph::removeNode(int id){

    if(searchNode(id))
    {
        Node* node = this->getFirstNode();
        Node* temp = nullptr;

        // busca pelo no desejado e do temp
        // para manter a integridade da lista
        while(node->getId() != id)
        {
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
        while(aux != nullptr)
        {
            aux->removeEdge(node, this->directed);
            aux = aux->getNextNode();
        }

        delete node;
        this->order--;
    }
    else
    {
        cout << "ERROR: No nao encontrado!";
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

int Graph::getTotalGroups() const {
    return total_groups;
}

void Graph::setTotalGroups(int totalGroups) {
    total_groups = totalGroups;
}

bool Graph::hasGroup(int groups[], int group){
    for(int i = 0; i < this->total_groups; i++){
        if(groups[i] == group){
            return true;
        }
    }
    return false;
}

bool Graph::addToGroup(int groups[], int group){
    if(!this->hasGroup(groups, group)){
        for(int i = 0; i < this->total_groups; i++){
            if(groups[i] == -1){
                groups[i] = group;
                return true;
            }
        }
        cout << "ERROR: Vetor de grupos de cheio!" << endl;
        return false;
    }
    cout << "ERROR: Vetor ja possui esse grupo!" << endl;
    return false;
}

bool Graph::graphHasGroup(int group){
    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        if(node->getGroup() == group){
            return true;
        }
    }
    return false;
}

//printar todos os nós com suas respectivas arestas no console
void Graph::print()
{
    // impressão de todas as arestas do grafo
    if(this->first_node != nullptr)
    {
        cout << "Imprimindo Grafo: " << endl;
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode())
        {
            cout << endl << "No: " << node->getId() << endl;
            cout << "Arestas do No: " << endl;
            int i = 1;
            int j = 1;
            for(Edge* aux = node->getFirstEdge(); aux != nullptr; aux = aux->getNextEdge())
            {
                if(aux->isMarked())
                {
                    cout << "Aresta de RETORNO " << j << ": ";
                    j++;
                } else
                    {
                        cout << "Aresta " << i << ": ";
                        i++;
                    }
                if(!aux->isDirected() || this->directed)
                {
                    cout << node->getId()  << " " << aux->getTargetId() << endl;
                }

            }
        }
    }
}

//função para salvar um grafo no arquivo de output na linguagem dot
void Graph::save(ofstream& output_file){
    string colors[5] = {"red", "blue", "orange", "yellow", "violet"};
    //definindo tipo de grafo para o dot
    string graphType = this->directed ? "strict digraph {" : "strict graph {";
    output_file << graphType << endl;

    //imprimindo todos os nós
    for(Node* aux = this->first_node; aux != nullptr; aux = aux->getNextNode())
    {
        output_file << aux->getId();

        if(aux->getGroup() >= 0 && aux->getGroup() < 5){
            output_file << "[color= " << colors[aux->getGroup()] << "]";
        }

        if(this->weighted_node)
        {
            output_file << " [weight=\"" << aux->getWeight() << "\"]";
        }
        output_file << ";" << endl;
    }

    //imprimindo todas as arestas
    if(this->directed){
        for(Node* node = this->first_node; node != nullptr; node = node->getNextNode())
        {
            for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge())
            {
                output_file << node->getId() << " -> " << edge->getTargetId();

                if(this->weighted_edge)
                {
                    output_file << " " << "[label=\"" << edge->getWeight() << "\",weight=\"" << edge->getWeight() << "\"]";
                }

                if(edge->isMarked())
                {
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

Graph* Graph::ArrayToGraph(int* nodes, int total_nodes){
    Graph* graph = new Graph(0, 0, 1, 0);
    for(int i = 0; i < total_nodes; i++){

        if(nodes[i] != -1){

            if(!graph->searchNode(i)){
                graph->insertNodeWithGroup(i, this->getNode(i)->getGroup());
            }

            if(nodes[i] != i){

                if(!graph->searchNode(nodes[i])){
                    graph->insertNodeWithGroup(nodes[i] , this->getNode(nodes[i])->getGroup());
                }

                graph->insertEdge(i, nodes[i], this->getNode(i)->hasEdgeBetween(nodes[i])->getWeight());
            }
        }
    }

    return graph;
}

float Graph::ArrayWeight(int* nodes, int total_nodes){
    float total = 0;

    for(int i = 0; i < total_nodes; i++){

        if(nodes[i] != -1 && nodes[i] != i){
            total += this->getNode(i)->hasEdgeBetween(nodes[i])->getWeight();
        }
    }

    return total;
}

int Graph::ArrayGroups(int* nodes, int total_nodes){
    int total = 0;
    for(int i = 0; i < total_nodes; i++){
        if(nodes[i] != -1){
            total += 1;

        }
    }
    return total;
}

Graph* Graph::primRandomizadoAGMG(float alpha, int iterations){
    int* parent = new int[this->getOrder()];
    int* node_groups = new int[this->getOrder()];
    int* groups = new int[this->getTotalGroups()+1];
    int* best = new int[this->getOrder()];
    bool first = true;
    float best_weight = 0;
    float total_weight = 0;
    float means;
    int current_iterations = 0;
    float fitness = 0;

    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        node_groups[node->getId()] = node->getGroup();
    }

    while(current_iterations < iterations){
        for(int i = 0; i < this->getOrder() && current_iterations < iterations; i++){
            current_iterations++;

            float current_weight = this->auxPrimRandomizado(i, alpha, parent, groups, node_groups);

                if(first){
                    first=false;
                    best_weight = current_weight;
                    for(int j = 0; j < this->getOrder(); j++)
                        best[j] = parent[j];

                } else if(best_weight > current_weight){
                    best_weight = current_weight;
                    for(int j = 0; j < this->getOrder(); j++)
                        best[j] = parent[j];

                }
                total_weight += current_weight;
                means = total_weight/current_iterations;
                fitness = best_weight/means;
        }
    }

    Graph* graph = this->ArrayToGraph(best, this->getOrder());

    delete []parent;
    delete []groups;
    delete []node_groups;
    delete []best;

    cout << "--------- Finalizando Randomizado: ---------" << endl << endl;
    cout << "melhor peso encontrado: " << best_weight << endl;
    cout << "qualidade do alfa: " << fitness << endl << endl;
    cout << "-----------------------------------------" << endl;


    return graph;
}

Graph* Graph::primGulosoAGMG(){
    int* parent = new int[this->getOrder()];
    int* best = new int[this->getOrder()];
    int* groups = new int[this->getTotalGroups()+1];
    int* node_groups = new int[this->getOrder()];
    float* weights = new float[this->getOrder()];
    float best_weight = 0;
    bool first = true;

    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        node_groups[node->getId()] = node->getGroup();
        best[node->getId()] = -1;
    }

    for(int i = 0; i < this->getOrder(); i++){
        float current_weight = this->auxPrimGuloso(i, parent, groups, node_groups, weights);
        if(first){
            first = false;
            best_weight = current_weight;
            for(int j = 0; j < this->order; j++)
                best[j] = parent[j];

        } else if(best_weight > current_weight){
            best_weight = current_weight;
            for(int j = 0; j < this->order; j++)
                best[j] = parent[j];
        }
    }

    Graph *graph = this->ArrayToGraph(best, this->getOrder());

    cout << "melhor peso encontrado: " << best_weight << endl;


    delete []parent;
    delete []groups;
    delete []best;
    delete []weights;
    delete []node_groups;

    return graph;

}


float Graph::auxPrimGuloso(int initial_node, int* parent, int* groups, int* node_groups, float* weights){

    int id_child, first, id_parent; //Variaveis de controle para a funcao
    float total_weight = 0;
    int total_nodes = this->getOrder();
    int chosen_nodes = 0;
    Node* current_node; //No atual da iteracao
    Edge* current_edge; //Aresta atual da iteracao
    float minimal_weight; //Variavel que armazena o peso do caminho atual
    //Vetor para guardar os nos pai de cada vértice da árvore
    for(int i = 0; i < total_nodes ; i++) //Inicializando vetor de pais
    {
        parent[i] = -1;
        weights[i] = -1;
    }

    for(int j = 0; j < this->total_groups+1; j++){
        groups[j] = -1;
    }


    parent[initial_node] = initial_node;
    groups[node_groups[initial_node]] = node_groups[initial_node];

    while (1)//Loop para definir o pai de cada vertice da arvore
    {
        first = 1; //Variavel de controle para continuar ou nao o loop
        for (Node* current_node = this->getFirstNode(); current_node != nullptr; current_node = current_node->getNextNode())//Loop para iterar entre todos os nos e gerar a arvore
        {

            if(parent[current_node->getId()] != -1) //Se o valor no vetor foi -1, nao tem pai, entao pode ser introduzido na arvore
            {
                current_edge = current_node->getFirstEdge(); //Pegando aresta que liga esse no a outro

                for (Edge* current_edge = current_node->getFirstEdge(); current_edge != nullptr; current_edge = current_edge->getNextEdge()) //Iterando entre todas as arestas desse no, para ver qual tem o menor peso
                {
                    if (parent[current_edge->getTargetId()] == -1 && groups[node_groups[current_edge->getTargetId()]] == -1) //Se o no que esta ligado ao da iteracao atual nao tiver pai, entra nessa condicional
                    {
                        if(first == 1) //Se for o primeio no a ser visitado entra nessa condicional
                        {
                            minimal_weight = current_edge->getWeight(); //Salva o peso dessa aresta
                            id_parent = current_node->getId(); //Salva esse no como pai
                            id_child = current_edge->getTargetId(); //Salva o outro no que a aresta aponta como filho
                            first = 0; //Coloca a variavel de controle como 0 para continuar o loop
                            weights[id_child] = current_edge->getWeight();
                        }
                        else //Se nao for o primeiro no
                        {
                            if (minimal_weight > current_edge->getWeight()) //Compara o peso da aresta atual com a menor, se o da atual for menor entra na condicao
                            {
                                minimal_weight = current_edge->getWeight(); //Salva o novo peso como menor peso
                                id_parent = current_node->getId(); //Salva o id do no atual como pai
                                id_child = current_edge->getTargetId(); //Salva o outro no que a aresta aponta como filho
                                weights[id_child] = current_edge->getWeight();
                            }
                        }
                    }
                }
            }
        }
        if(first == 1){ //Se first node chegar como 1 significa que não tem mais arestas esse nó
            break;
        }

        if(id_child != -1){
            parent[id_child] = id_parent; //Salva o no pai no indice do filho no vetor, para continuar adequadamente o algoritmo
            int child_group = node_groups[id_child];
            groups[child_group] = child_group;
            total_weight += weights[id_child];
            chosen_nodes++;
        }

    }

    return total_weight;
}

float Graph::auxPrimRandomizado(int initial_node, float alpha, int* parent, int* groups, int* node_groups){

    //------------- Declarando variáveis e arrays -------------

    int id_child, id_parent;
    int total_nodes = this->getOrder();
    bool *nodes = new bool[this->getOrder()];
    float* weights = new float[this->getOrder()];
    float total_weight = 0;



    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode())
    {
        parent[node->getId()] = -1;
        weights[node->getId()] = std::numeric_limits<float>::max();
    }

    for(int j = 0; j < this->total_groups+1; j++){
        groups[j] = -1;
    }

    //definindo vertice e grupo iniciais
    parent[initial_node] = initial_node;
    groups[node_groups[initial_node]] = node_groups[initial_node];

    //-------------------------------------------------------

    //-------- Loop Principal do prim randomizado -----------

    while (1)//Loop para definir o pai de cada vertice da arvore
    {
        bool check = true;

        //LCR = lista de candidatos restrita
        //busca pelo LCR no grafo, retorna falso para caso nao tenha encontrado ninguem para compor o LCR
        check = this->nodeRange(parent, groups, node_groups, nodes, alpha, weights);

        //caso a LCR esteja vazia finaliza a iteração atual
        if(!check){
            break;
        }


        //escolhe randomicamente um vértice da LCR e usa a busca sua menor aresta para a solução
        id_child = this->randNode(nodes);
        id_parent = this->randEdge(parent, groups, id_child);

        //atualiza lista de grupos e de vizinhança, assim como o peso
        if(id_child != -1){
            parent[id_child] = id_parent;
            int child_group = node_groups[id_child];
            groups[child_group] = child_group;
            total_weight += weights[id_child];
        }

    }

    //-------------------------------------------------------

    return total_weight;
}

int Graph::randEdge(int* parent, int* groups, int node){
    Node* current_node;
    Edge* current_edge;
    float minimal_weight, current_weight;
    bool first = true;
    int chosen_node = -1;

    for (int i = 0; i < this->getOrder(); i++)
    {
        if(parent[i] != -1)
        {
            current_node = this->getNode(i);
            if(current_node->searchEdge(node)){
                current_weight = current_node->hasEdgeBetween(node)->getWeight();
                if(first){
                    minimal_weight = current_weight;
                    chosen_node = current_node->getId();
                    first = false;
                } else if(minimal_weight > current_weight){
                    minimal_weight = current_weight;
                    chosen_node = current_node->getId();
                }
            }
        }
    }

    return chosen_node;
}


//função que cria pelos possíveis nós da LCR cria um array desses e sortei um deles, retornando seu ID
int Graph::randNode(bool* nodes){
    int size = 0;
    int chosen_node = -1;
    for(int i =0; i < this->getOrder(); i++){
        if(nodes[i])
            size++;
    }

    if(size != 0){
        int* chosenNodes = new int[size];

        int j = 0;
        for(int i =0; i < this->getOrder(); i++){
            if(nodes[i]){
                chosenNodes[j] = i;
                j++;
            }
        }

        chosen_node = chosenNodes[rand()%size];
    }

    return chosen_node;
}

//função responsavel por buscar e retornar a LCR
bool Graph::nodeRange(int* parent, int* groups, int* node_groups, bool* nodes, float alpha, float* weights) {

    //-----------declaração de variáveis-----------




    int id_child, first, id_parent;
    int total_nodes = this->getOrder();
    float minimal_weight = std::numeric_limits<float>::max();
    float maximum_weight = std::numeric_limits<float>::min();

    for (int j = 0; j < this->getOrder(); j++) {
        nodes[j] = false;
    }

    //-------------------------------------------------------------------------------------------

    //--------------------- Busca por valores de maior e menor distância ------------------------

    first = 1;
    for (Node* current_node = this->getFirstNode(); current_node != nullptr; current_node = current_node->getNextNode()) {
        if (parent[current_node->getId()] != -1) {
            for (Edge* current_edge = current_node->getFirstEdge(); current_edge != nullptr; current_edge = current_edge->getNextEdge()) {
                if (parent[current_edge->getTargetId()] == -1 && groups[node_groups[current_edge->getTargetId()]] == -1) {
                    if (minimal_weight > current_edge->getWeight()) //Compara o peso da aresta atual com a menor, se o da atual for menor entra na condicao
                    {
                        minimal_weight = current_edge->getWeight(); //Salva o novo peso como menor peso
                    }

                    if (maximum_weight < current_edge->getWeight()) //Compara o peso da aresta atual com a menor, se o da atual for menor entra na condicao
                    {
                        maximum_weight = current_edge->getWeight(); //Salva o novo peso como menor peso
                    }
                }
            }

        }
    }

    //variavel para cehcar se algum local pode ser visitado ou não -- condição de parada para o prim

    if(minimal_weight != std::numeric_limits<float>::max() && maximum_weight != std::numeric_limits<float>::min()) {
        //------------------------------------------------------------------------------------------


        //definindo variável de distância maxima randomizada por alpha
        float maxDistance = minimal_weight + (alpha * (maximum_weight - minimal_weight));




        //-----------------------Busca pelos nos que se adequam a distancia minima-----------------------------

        for (Node* current_node = this->getFirstNode(); current_node != nullptr; current_node = current_node->getNextNode()) {

            if (parent[current_node->getId()] != -1) {
                //busca pelas arestas/nos possiveis do vertice
                for (Edge* current_edge = current_node->getFirstEdge(); current_edge != nullptr; current_edge = current_edge->getNextEdge()) {
                    int index = current_edge->getTargetId();

                    //checagem se eh valido!
                    if (current_edge->getWeight() <= maxDistance && parent[current_edge->getTargetId()] == -1 && groups[node_groups[current_edge->getTargetId()]] == -1) {

                        //atualizando array de nos e pesos
                        if(weights[index] > current_edge->getWeight()){
                            weights[index] = current_edge->getWeight();
                        }

                        nodes[index] = true;

                    }
                }
            }
        }

        return true;

    }

    return false;

    //---------------------------------------------------------------------------------

}


Graph* Graph::primReativoAGMG(float* alpha, int alpha_size, int iterations, int block){

    //---------------------- Declarando variaveis/arrays ----------------------------------------

    int current_iterations = 0;
    double* total_weight = new double[alpha_size];
    int* alpha_iterations = new int[alpha_size];
    float* probability = new float[alpha_size];
    float* fitness = new float[alpha_size];
    float best_fitness;
    float best_weight;
    float total_fitness = 0;
    int* node_groups = new int[this->getOrder()];
    int* parent = new int[this->getOrder()];
    int* best = new int[this->getOrder()];
    int* current_best = new int[this->getOrder()];

    for(Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        node_groups[node->getId()] = node->getGroup();
    }

    bool check = true;

    //---------------------------------------------------------------------------------


    //preenchendo valores mínimos para evitar divisão por 0
    //esses valores são: qualidade de solução (fitness)
    //melhor peso e melhor qualidade (best_weight e best_fitness)

    for(int i = 0; i < alpha_size; i++){

        //variaveis de controle
        int localIterations = 0;
        double localWeight = 0;
        float chosen_weight = this->auxPrimReativo(node_groups, parent, current_best, alpha[i], 1, &localIterations, &localWeight);
        alpha_iterations[i] += localIterations;
        current_iterations += localIterations;
        total_weight[i] = localWeight;

        if(check){
            check = false;
            best_weight = chosen_weight;
            for(int j = 0; j < this->getOrder(); j++)
                best[j] = current_best[j];

        } else if(best_weight > chosen_weight){
            best_weight = chosen_weight;
            for(int j = 0; j < this->getOrder(); j++)
                best[j] = current_best[j];
        }
    }


    //------------------- while loop principal do reativo ---------------------

    check = true;
    while(current_iterations < iterations) {


        //resetando variaveis locais
        int block_iteration = 0;
        total_fitness = 0;

        //-------------- loop para setar qualidade (fitness) -------------------

        //busca pela melhor qualidade de um alfa ao mesmo tempo que calcula a soma de total desses
        for (int i = 0; i < alpha_size; i++) {


            fitness[i] = best_weight / (total_weight[i] / alpha_iterations[i]);
            total_fitness += fitness[i];

            if (check) {
                best_fitness = fitness[i];
            } else if (best_fitness > fitness[i]) {
                best_fitness = fitness[i];
            }
        }

        //-------------------------------------------------------------------


        //----------- loop para escolher alfa randomizado -------------------

        //definindo seed de randomização
        float chosen = rand()/(RAND_MAX + 1.0);
        float chosen_probability = 0;
        int chosen_index = -1;

        //reseta qualidade da solução ao mesmo tempo que decide qual será a nova
        //não interfere no resultado pois antes de decidir se um alfa é ou não escolhido sua
        //qualidade ja foi atualizada
        for (int i = 0; i < alpha_size; i++) {
            probability[i] = fitness[i] / total_fitness;
            chosen_probability += probability[i];
            if (chosen_index == -1 && chosen_probability > chosen) {
                chosen_index = i;
            }
        }



        //-------------------------------------------------------------------


        //atualização no console dos princiaps valores por atualização de bloco de iteraçoes

        cout << "----------  Bloco resetado  ------------" << endl;
        cout << "alfa escolhido: " << alpha[chosen_index] << endl;
        cout << "fitness do alfa escolhido: " << fitness[chosen_index] << endl;
        cout << "probabilidade do alfa  1: " << probability[0] << endl;
        cout << "probabilidade do alfa  2: " << probability[1] << endl;
        cout << "probabilidade do alfa  3: " << probability[2] << endl;
        cout << "probabilidade do alfa  4: " << probability[3] << endl;
        cout << "probabilidade do alfa  5: " << probability[4] << endl;
        cout << "rand: " << chosen << endl;
        cout << "melhor peso atual: " << best_weight << endl;
        cout << "----------------------------------------" << endl;

        //-------------------------------------------------------------------


        //----------------------- while loop por bloco de iteração  -----------------------------

        while (block_iteration < block && current_iterations < iterations) {




            //busca por nova solução e verificação se é a nova melhor
            //função retorna iteraçoes realizadas no alfa
            //definindo variaveis de maximo de iteraçoes e int que será usado como contador
            int doneIterations = 0;
            double weightFound = 0;
            int maxIterations = iterations-current_iterations < block-block_iteration ? iterations-current_iterations : block-block_iteration;


            float current_weight = this->auxPrimReativo(node_groups, parent, current_best, alpha[chosen_index], maxIterations, &doneIterations, &weightFound);

            //atualizacao dos valores de peso total do alfa e iteracoes desse alfa
            total_weight[chosen_index] += weightFound;
            current_iterations += doneIterations;
            block_iteration += doneIterations;
            alpha_iterations[chosen_index] += doneIterations;

            //checagem para caso seja o melhor peso
            if (best_weight > current_weight) {
                best_weight = current_weight;
                for(int j = 0; j < this->getOrder(); j++)
                    best[j] = current_best[j];
            }

        }

        //---------------------------------------------------------------------------------------

    }

    //-------------------- impressao no console de resultados finais ---------------------------

    for(int i = 0; i < alpha_size; i++){
        cout << "alfa " << i << " " << alpha[i] << endl;
        cout << "fitness: "  << fitness[i] << endl;
        cout << "probabilidade: " << probability[i] << endl << endl;
    }

    cout << "melhor peso: " << best_weight << endl;
    cout << "melhor fitness: " << best_fitness << endl << endl;

    //----------------------------------------------------------------------------------------

    //transformando o melhor resultado em um grafo
    Graph* graph = this->ArrayToGraph(best, this->getOrder());

    //---------------------- deletando arrays criados dinamicamente --------------------------
    delete []probability;
    delete []fitness;
    delete []total_weight;
    delete []alpha_iterations;
    delete []node_groups;
    delete []parent;
    delete []best;
    delete []current_best;

    return graph;

}

float Graph::auxPrimReativo(int* node_groups, int* parent, int* best, float alpha, int max_iterations, int* doneiterations, double* weightFound){

    //---------------------- Declarando variaveis/arrays ----------------------------------------

    int* groups = new int[this->getTotalGroups()+1];
    float best_weight = 0;

    bool first = true;

    //------------------------------------------------------------------------------------

    //------------------------- for loop principal ---------------------------------------

    for(int i = 0; i < this->getOrder() && (*doneiterations) < max_iterations; i++){
        (*doneiterations)++;

        //chamando prim guloso randomizado com alpha passado e arrays de visitacao
        //de vertices e grupos e o vertice inicial
        float current_weight = this->auxPrimRandomizado(i, alpha, parent, groups, node_groups);
        //verificação da validadez da solução
        (*weightFound) += current_weight;

        if(first){
            first = false;
            best_weight = current_weight;
            for(int j = 0; j < this->getOrder(); j++)
                best[j] = parent[j];

        } else if(best_weight > current_weight){
            best_weight = current_weight;
            for(int j = 0; j < this->getOrder(); j++)
                best[j] = parent[j];
        }

    }

    //------------------------------------------------------------------------------------

    delete []groups;

    return best_weight;
}





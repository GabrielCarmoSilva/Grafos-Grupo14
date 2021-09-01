#include "Node.h"
#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Node's methods
**************************************************************************************************/

// Constructor
Node::Node(int id){

    this->id = id;
    this->in_degree = 0;
    this->out_degree = 0;
    this->weight = 0;
    this->first_edge = nullptr;
    this->last_edge = nullptr;
    this->next_node = nullptr;

};

// Destructor
Node::~Node(){

    Edge* next_edge = this->first_edge;

    //destruindo todas as suas arestas
    while(next_edge != nullptr){

        Edge* aux_edge = next_edge->getNextEdge();
        delete next_edge;
        next_edge = aux_edge;

    }

};

// Getters
Edge* Node::getFirstEdge(){

    return this->first_edge;

}

Edge* Node::getLastEdge(){

    return this->last_edge;

}

int Node::getId(){

    return this->id;

}

int Node::getInDegree(){

    return this->in_degree;

}

int Node::getOutDegree(){

    return this->out_degree;

}

float Node::getWeight(){

    return this->weight;

}

Node* Node::getNextNode(){

    return this->next_node;

}

// Setters

void Node::setNextNode(Node* next_node){

    this->next_node = next_node;

}

void Node::setWeight(float weight){

    this->weight = weight;

}

int Node::getGroup() const {
    return group;
}

void Node::setGroup(int group) {
    Node::group = group;
}

// Other methods
void Node::insertEdge(Node* target_node, bool directed,  float weight){
    // Verifies whether there are at least one edge in the node
    Edge* edge = new Edge(target_node->getId());
    edge->setWeight(weight);

    if(this->first_edge != nullptr){
        // caso não seja a primeira

        this->last_edge->setNextEdge(edge);
        this->last_edge = edge;

    }
    else{
        // caso seja a primeira aresta
        this->first_edge = edge;
        this->last_edge = this->first_edge;

    }

    if(directed){

        //atualizando valores de entrada e saida e setando a aresta como direcionada
        this->incrementOutDegree();
        target_node->incrementInDegree();
        edge->setDirected(true);

    } else{

        //utilizando uma aresta direcionada como aresta de volta
        //para o grafo nao direcionado
        //assim evita-se um loop infinito e tem-se uma diferenciação para essas arestas auxiliares

        this->incrementOutDegree();
        if(this->id != target_node->getId())
        {
            target_node->insertEdge(this, 1, weight);

            //como é utilizado o modo de aresta direcionada é preciso atualizar novamento os valores
            this->decrementInDegree();
        }
        else{
            //caso seja self loop
            this->incrementOutDegree();
        }
    }

}

void Node::removeAllEdges(){
    if(this->first_edge != nullptr){

        // removendo todas as arestas que partem do nó
        Edge* next_edge = this->first_edge;
        while(next_edge != nullptr){
            Edge* aux_edge = next_edge->getNextEdge();
            delete next_edge;
            next_edge = aux_edge;
        }


    }

    //setando atributos apos deletar as arestas
    this->out_degree = 0;
    this->first_edge = this->last_edge = nullptr;

}

int Node::removeEdge(Node* target_node, bool directed){
    // busca e verifica se a aresta de fato existe
    int id = target_node->getId();
    if(this->searchEdge(id)){

        Edge* aux = this->first_edge;
        Edge* previous = nullptr;
        // procurando pela aresta a ser removida
        //a variável previous é necessária para manter a integridade da lista
        while(aux->getTargetId() != id){
            previous = aux;
            aux = aux->getNextEdge();
        }
        if(previous != nullptr)
            previous->setNextEdge(aux->getNextEdge());
        else
            this->first_edge = aux->getNextEdge();

        if(aux == this->last_edge)
            this->last_edge = previous;

        if(aux->getNextEdge() == this->last_edge)
            this->last_edge = aux->getNextEdge();

        delete aux;

        //fazendo as devidas mudanças de valor para caso seja direcionado ou não
        if(directed){
            this->decrementOutDegree();
            target_node->decrementInDegree();
        } else if(id != this->getId()){
            this->decrementOutDegree();
            this->incrementInDegree();

            //como existem arestas auxiliares que foram setadas como direcionadas
            //para os grafos nao direciondas
            // ao remover uma aresta desses grafos eh preciso também remover sua auxiliar

            target_node->removeEdge(this, 1);

        } else{
            //caso seja self loop
            this->decrementOutDegree();
            this->decrementOutDegree();
        }

        return 1;

    }

    return 0;

}

bool Node::searchEdge(int target_id){
    // Verifies whether there are at least one edge in the node
    if(this->first_edge != nullptr){
        // Searching for a specific edge of target id equal to target id
        for(Edge* aux = this->first_edge; aux != nullptr; aux = aux->getNextEdge()){
            if(aux->getTargetId() == target_id)
                return true;
        }
    }

    return false;

}


void Node::incrementInDegree(){

    this->in_degree++;

}

void Node::incrementOutDegree(){

    this->out_degree++;

}

void Node::decrementInDegree(){

    this->in_degree--;

}

void Node::decrementOutDegree(){

    this->out_degree--;

}

Edge* Node::hasEdgeBetween(int target_id)
{

    for(Edge *auxEdge = this->first_edge; auxEdge != nullptr; auxEdge = auxEdge->getNextEdge())
    {
        if(auxEdge->getTargetId() == target_id)
            return auxEdge;
    }
    return nullptr;
}


//
// Created by davi2 on 01/09/2021.
//

#include "Solution.h"

Solution::Solution(int *nodes, float weight, int total_nodes){

    int* myNodes = new int[total_nodes];

    for(int i = 0; i < total_nodes; i++){
        myNodes[i] = nodes[i];
    }

    this->nodes = myNodes;
    this->weight = weight;
    this->total_nodes = total_nodes;
}

Solution::~Solution() {
}

int *Solution::getNodes() const {
    return nodes;
}

void Solution::setNodes(int *nodes) {
    Solution::nodes = nodes;
}

float Solution::getWeight() const {
    return weight;
}

void Solution::setWeight(float weight) {
    Solution::weight = weight;
}

int Solution::getTotalNodes() const {
    return total_nodes;
}

void Solution::setTotalNodes(int totalNodes) {
    total_nodes = totalNodes;
}

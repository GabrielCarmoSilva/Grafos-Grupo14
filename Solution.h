//
// Created by davi2 on 01/09/2021.
//

#ifndef GRAFOS_GRUPO14_SOLUTION_H
#define GRAFOS_GRUPO14_SOLUTION_H


class Solution {

private:
    int* nodes;
    float weight;
    int total_nodes;

public:
    Solution(int *array, float weight, int total_nodes);

    virtual ~Solution();

    int *getNodes() const;

    void setNodes(int *nodes);

    float getWeight() const;

    void setWeight(float weight);

    int getTotalNodes() const;

    void setTotalNodes(int totalNodes);


};


#endif //GRAFOS_GRUPO14_SOLUTION_H

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variaveis para auxiliar na criacao dos nao no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if(!graph->getWeightedEdge() && !graph->getWeightedNode())
    {

        while(input_file >> idNodeSource >> idNodeTarget)
        {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }

    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() )
            {

                float edgeWeight;

                while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight)
                {

                    graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

                }

            }else if(graph->getWeightedNode() && !graph->getWeightedEdge())
                    {

                        float nodeSourceWeight, nodeTargetWeight;

                        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight)
                        {

                            graph->insertEdge(idNodeSource, idNodeTarget, 0);
                            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
                            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

                        }

                    }else if(graph->getWeightedNode() && graph->getWeightedEdge())
                            {

                                float nodeSourceWeight, nodeTargetWeight, edgeWeight;

                                while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight >> edgeWeight)
                                {

                                    graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
                                    graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
                                    graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

                                }

                            }
    return graph;
}


Graph* leituraAGMG(ifstream& input_file){

    // o AGMG é sempre não direcionado e com peso nas arestas
    Graph* graph = new Graph(0, 0, 1, 0);

    int idNodeSource;
    string group;
    int currentVertex = 0;
    int idNodeTarget;
    float edgeWeight;
    int total_groups = 0;

    while(getline(input_file, group) && !group.empty()){

        if(!graph->graphHasGroup(stoi(group))){
            total_groups++;
        }

        graph->insertNodeWithGroup(currentVertex, stoi(group));
        currentVertex++;

    }

    cout << "numero total de grupos " << total_groups << endl;

    graph->setTotalGroups(total_groups);

    while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight)
    {
        graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
    }

    return graph;
}

int menu(){
    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "Favor, utilize os vértices numerados de 0 a n-1" << endl;
    cout << "Árvore Geradora Mínima Generalizada" << endl;
    cout << endl;
    cout << "[1] Algoritmo Guloso - Prim" << endl;
    cout << "[2] Algoritmo Randomizado - Prim" << endl;
    cout << "[3] Algoritmo Randomizado Reativo - Prim" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;
}

bool salvar(){
    int resposta = 0;
    while(true)
    {
        cout << "Voce deseja salvar o grafo resultante deste algoritmo?" << endl;
        cout << "[1] Sim" << endl;
        cout << "[2] Nao" << endl;
        cin >> resposta;
        if(resposta == 1 || resposta == 2)
        {
            break;
        } else
          {
            cout << "Resposta invalida! Por favor digite novamente." << endl;
          }
    }
    return resposta == 1;
}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    ofstream saving_file;
    saving_file.open("tests.txt", ios::out | ios::trunc);

    saving_file << "-------ALGORITMO GULOSO-----------" << endl;
    auto start = high_resolution_clock::now();
    graph->primGulosoAGMG(saving_file);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
            saving_file << "Tempo de execução da função: "
                    << duration.count() / pow(10, 6) << " seconds" << endl;

    saving_file << endl;

    float* alpha = new float[5]{0.5, 0.3, 0.15, 0.1, 0.05};
    int i = 0;    
    saving_file << "-------ALGORITMO RANDOMIZADO-------" << endl;
    while(i < 10) {
        saving_file << "Iteração " << i+1 << endl;
        for(int j = 0; j < 5; j++) {
            start = high_resolution_clock::now();
            graph->primRandomizadoAGMG(alpha[j], 500, saving_file);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
                    saving_file << "Tempo de execução da função: "
                            << duration.count() / pow(10, 6) << " seconds" << endl; 
                saving_file << endl;
        }
        i++;
        saving_file << endl;
    }    

    i = 0;
    saving_file << endl;
    saving_file << "-------ALGORITMO RANDOMIZADO REATIVO-----" << endl;
    while(i < 10) {
        saving_file << "Iteração " << i+1 << endl;
        start = high_resolution_clock::now();
        graph->primReativoAGMG(alpha, 5, 5000, 1000, saving_file);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        saving_file << "Tempo de execução da função: "
                << duration.count() / pow(10, 6) << " seconds" << endl; 
        i++;
        saving_file << endl;
    }

    switch (selecao)
    {
        //Sair
        case 0:
        {
            cout << "Saindo!" << endl;
            exit(0);
            break;
        }
        //Árvore geradora mínima generalizada - algoritmo de Prim guloso
        case 1:
        {
            srand(time(0));
            auto start = high_resolution_clock::now();
            graph->primGulosoAGMG(saving_file);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Tempo de execução da função: "
                    << duration.count() / pow(10, 6) << " seconds" << endl;
            break;
        }
        //Árvore geradora mínima generalizada - algoritmo de Prim randomizado
        case 2:
        {
            srand(time(0));
            float alpha = 0.0;
            int numIteracoes = 0;
            cout << "Qual alfa você escolhe?" << endl;
            cin >> alpha;
            cout << "Digite o número de iterações" << endl;
            cin >> numIteracoes;
            auto start = high_resolution_clock::now();
            graph->primRandomizadoAGMG(alpha, numIteracoes, saving_file);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Tempo de execução da função: "
                    << duration.count() / pow(10, 6) << " seconds" << endl;
            break;
        } 
        //Árvore geradora mínima generalizada - algoritmo de Prim randomizado reativo
        case 3:
        {
            srand(time(0));
            int numIteracoes, bloco = 0;
            cout << "Digite o número de iterações total" << endl;
            cin >> numIteracoes;
            cout << "Digite o número de iterações por bloco" << endl;
            cin >> bloco;
            float* alpha = new float[5]{0.5, 0.3, 0.15, 0.1, 0.05};
            auto start = high_resolution_clock::now();
            graph->primReativoAGMG(alpha, 5, numIteracoes, bloco, saving_file);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Tempo de execução da função: "
                    << duration.count() / pow(10, 6) << " seconds" << endl;
            break;
        }
        default:
        {
            cout << "Erro! Opcao invalida." << endl;
        }

    }

}

int mainMenu(ofstream& output_file, Graph* graph){
    int selecao = 1;

    while(selecao != 0)
    {
        //system("cls");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Impossibilitado de abrir o arquivo de saida(output_file)!"<< endl;

        output_file << endl;

    }
    return 0;
}



int main(int argc, char const *argv[]) {

    //Verifica se todos os parametros do programa foram entrados
    if (argc != 3)
    {
        cout << "Erro! Esperado: ./<program_name> <input_file> <output_file>" << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if(input_file_name.find("v") <= input_file_name.size())
    {
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Rodando " << program_name << " com instancia " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);


    Graph* graph;
    if(input_file.is_open())
    {
        graph = leituraAGMG(input_file);
    }else
        cout << "Impossibilitado de abrir " << argv[1];

    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saida
    output_file.close();

    return 0;
}

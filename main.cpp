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

using namespace std;

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
    cout << "Favor, utilize os vértices numerados de 1 a n" << endl;
    cout << endl;
    cout << "[1] Fecho transitivo direto do vertice de um grafo direcionado" << endl;
    cout << "[2] Fecho transitivo indireto do vertice de um grafo direcionado" << endl;
    cout << "[3] Caminho minimo entre dois vertices - Dijkstra" << endl;
    cout << "[4] Caminho minimo entre dois vertices - Floyd" << endl;
    cout << "[5] Arvore Geradora minimo de Prim" << endl;
    cout << "[6] Arvore Geradora minimo de Kruskal" << endl;
    cout << "[7] Arvore dada pela ordem de caminhamento em profundidade, destacando as arestas de retorno" << endl;
    cout << "[8] Ordenacao topologica do grafo aciclico direcionado D" << endl;
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

    switch (selecao)
    {
        //Sair
        case 0:
        {
            srand(time(0));
           // Graph* aux = graph->PrimAGMG(1);
            Graph* aux = graph->primGulosoAGMG();
            //Graph* aux2 = graph->primRandomizadoAGMG(0.5);
            float* alpha = new float[5]{0.5, 0.3, 0.15, 0.1, 0.05};
            graph->primRandomizadoAGMG(0.1, 1000);
           // graph->primReativoAGMG(alpha, 5, 100000, 10000);
            //aux->save(output_file);
            //aux2->save(output_file);
            exit(0);
            break;
        }
        //Fecho transitivo direto do vertice de um grafo direcionado
        case 1:
        {
            int id;
            cout << "Digite o id do vertice: " << endl;
            cin >> id;
            Graph* aux = graph->FTD(id);

            if(aux != nullptr && salvar())
                aux->save(output_file);

            break;
        }
        //Fecho transitivo indireto do vertice de um grafo direcionado
        case 2:
        {
            int id;
            cout << "Digite o id do vertice: " << endl;
            cin >> id;
            Graph* aux = graph->FTI(id);

            if(aux != nullptr && salvar())
                aux->save(output_file);

            break;
        }
        //Caminho minimo entre dois vertices - Dijkstra
        case 3:
        {
            int idSource;
            int idTarget;
            cout << "Digite o id de origem: " << endl;
            cin >> idSource;
            cout << "Digite o id de destino: " << endl;
            cin >> idTarget;
            Graph* aux = graph->dijkstra(idSource, idTarget);

            if(aux != nullptr && salvar())
                aux->save(output_file);

            break;
        }
        //Caminho minimo entre dois vertices - Floyd
        case 4:{
            int idSource;
            int idTarget;
            while (!graph->searchNode(idSource))
            {
                cout << "Digite o id do primeiro vertice" << endl;
                cin >> idSource;
            }
            while (!graph->searchNode(idTarget))
            {
                cout << "Digite o id do outro vertice" << endl;
                cin >> idTarget;
            }
            Graph* aux = graph->floydMarshall(idSource, idTarget);
            if(aux != nullptr && salvar())
                aux->save(output_file);
            break;
        }
        //Arvore Geradora minimo de Prim
        case 5:
        {
            int n;
            cout << "Digite a quantidade de vertices" << endl;
            cin >> n;
            int *nodes = new int[n];
            for (int i = 0; i < n; i++)
            {
                while (!graph->searchNode(nodes[i]))
                {
                    cout << "Digite o vertice " << i << ":" << endl;
                    cin >> nodes[i];
                }
                
            }
            Graph* aux = graph->agmPrim(n, nodes);
            if(aux != nullptr && salvar())
                aux->save(output_file);
            break;
        }
        //Arvore Geradora minimo de Kruskal
        case 6:
        {
            int n;
            cout << "Digite a quantidade de vertices" << endl;
            cin >> n;
            int *nodes = new int[n];
            for (int i = 0; i < n; i++)
            {
                while (!graph->searchNode(nodes[i]))
                {
                    cout << "Digite o vertice " << i << ":" << endl;
                    cin >> nodes[i];
                }
                
            }
            Graph* aux = graph->agmKruskal(n, nodes);
            if(aux != nullptr && salvar())
                aux->save(output_file);
            break;
        }
        //Arvore dada pela ordem de caminhamento em profundidade, destacando as arestas de retorno
        case 7:
        {
            int n;
            cout << "Escolha o vértice inicial: " << endl;
            cin >> n;
            Graph* aux = graph->BuscaEmProfundidade(n);

            if(aux != nullptr && salvar())
                aux->save(output_file);

            break;
        }
        //Ordenacao topologica do grafo aciclico direcionado D
        case 8:
        {
            Graph* aux = graph->aciclicoDirecionado();

            if(aux != nullptr && salvar())
                aux->save(output_file);

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

# Problema da Árvore Geradora Mínima Generalizada
## Aplicação da Disciplina de Teoria de Grafos da UFJF.

<!--ts-->
   * [Desenvolvedores](#desenvolvedores)
   * [Professores](#professores)
   * [Tecnologias](#tecnologias)
   * [Instalação](#instalação)
   * [Pré-requisitos para utilização](#pré-requisitos-para-utilização)
   * Utilização
      * [Preparação de ambiente](#preparação-de-ambiente)
      * [Compilação](#compilação)
      * [Execução](#execução)
<!--te-->

### Desenvolvedores:
- Anna Letícia Franco Monteiro - 202065106A 
- Davi Esteves dos Santos - 202065504B
- Gabriel do Carmo Silva - 202065030A
- Pedro Campos Lima - 202065521B

### Professores:
- Stênio Sã Rosário Furtado Soares
- Luciana Brugiolo Gonçalves

### Tecnologias

As seguintes ferramentas foram usadas na construção do projeto:
- [C++](https://isocpp.org/)

### Instalação:

- Basta clonar ou fazer download do arquivos do repositório.

Para clonar abra o terminal na pasta desejada e digite o comando:
```
git clone https://github.com/GabrielCarmoSilva/Grafos-Grupo14.git
```
### Pré-requisitos para utilização:

Antes de começar, você vai precisar ter instalado em sua máquina as seguintes ferramentas: [Git](https://git-scm.com) e [C++](https://isocpp.org/). 
É necessário também um arquivo em branco no formato [.dot](https://www.reviversoft.com/pt/file-extensions/dot) de nome qualquer para guardar o resultado gerado.
Além disto é interessante uma IDE que suporte a linguagem, como [VSCode](https://code.visualstudio.com/) ou [CLion](https://www.jetbrains.com/pt-br/clion/).

### Utilização:
#### Preparação de ambiente:
Após clonar o repositório ou descompactar o .zip enviado, os arquivos contendo os grafos a serem lidos estarão na pasta gmstInstances. Para compilar e executar o programa e receber a saída esperada ao ler um grafo, siga as instruções abaixo. 

#### Compilação
Em um terminal do seu sistema operacional, rode o seguinte comando: ```g++ -o *.c* execGrupo14```. O programa será compilado e estará pronto para executar.

#### Execução
Ainda no terminal, utilize o comando ```./execGrupo14 input.txt output.dot```. Substitua ```input.txt``` pelo nome da instância que você deseja rodar. Exemplo: para a instãncia 10att48.clu, substitua ```input.txt``` por ```gmstInstances/10att48.clu```. Já o arquivo ```output.dot``` é um arquivo que deixamos em branco por padrão para salvar os grafos gerados. Sinta-se livre para criar um novo arquivo e salvar os grafos neste. Será mostrado um menu com algumas opções, onde cada uma delas é correspondente a um dos algoritmos, e existe também uma opção para parar de executar o programa.

 ###### Algoritmo Guloso
 Para executar o algoritmo guloso no grafo em que você leu, digite 1. Ele informará o melhor peso e o tempo de execução na tela e perguntará se você deseja salvar a árvore geradora mínima encontrada no arquivo de saída passado por argumento. Digite 1 para salvar, e 2 para não salvar.
 
 ###### Algoritmo Guloso Randomizado
Para executar o algoritmo guloso randomizado no grafo em que você leu, digite 2. Ele perguntará qual alfa você deseja usar no algoritmo, e quantas iterações você deseja que o algoritmo faça. Digite essas informações na tela, e ele informará o melhor peso e o tempo de execução na tela, e perguntará se você deseja salvar a árvore geradora mínima encontrada no arquivo de saída passado por argumento. Digite 1 para salvar, e 2 para não salvar. 
 
 ###### Algoritmo Guloso Randomizado Reativo
Para executar o algoritmo guloso randomizado reativo no grafo em que você leu, digite 3. Ele perguntará qual o número total de iterações que você deseja que o algoritmo faça, além do número de iterações por bloco. Em relação aos alfas, ele usará um vetor com cinco alfas informado na especificação (0.5, 0.3, 0.15, 0.1, 0.05). Digite essas informações na tela, e ele informará o melhor peso e o tempo de execução na tela, e perguntará se você deseja salvar a árvore geradora mínima encontrada no arquivo de saída passado por argumento. Digite 1 para salvar, e 2 para não salvar. 

 ###### Sair
 Digite 0 para parar a execução do programa.

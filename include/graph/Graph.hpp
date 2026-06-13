/*

Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

Graph.hpp

Responsável por representar o Grafo de Coocorrência construído
a partir dos documentos processados pelo SpacyProcessor.

Fluxo dos dados:

1. O CorpusManager encontra os arquivos do corpus.

2. O FileLoader lê cada arquivo e devolve seu conteúdo
   como uma string.

3. O SpacyProcessor recebe a string, remove stopwords,
   pontuações e realiza a lematização.

   Exemplo:

       Texto:
       "sigma aura sigma rizz"

       Saída:
       {"sigma", "aura", "sigma", "rizz"}

4. O GraphBuilder recebe os tokens processados e gera
   as coocorrências entre palavras.

   Exemplo:

       {"sigma", "aura", "sigma", "rizz"}

       Após remover duplicatas:

       {"sigma", "aura", "rizz"}

       Coocorrências geradas:

       sigma - aura
       sigma - rizz
       aura  - rizz

5. O GraphBuilder contabiliza as ocorrências utilizando
   uma estrutura de hash (edgeCounter).

6. O método build() do GraphBuilder cria o objeto Graph
   adicionando vértices e arestas ponderadas.

Representação do grafo:

- Cada vértice representa uma palavra (lemma).
- Cada aresta representa uma coocorrência.
- O peso da aresta representa em quantos documentos
  aquela associação apareceu.

Exemplo:

Documento 1:
    sigma aura rizz

Documento 2:
    sigma aura

Resultado:

       (2)
 sigma ----- aura
    \
     \
    (1)
       \
        rizz

Onde:

sigma-aura = 2
sigma-rizz = 1
aura-rizz  = 1
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Edge.hpp"

class Graph
{
private:                                                              //          {from       , to e weight}
    std::unordered_map<std::string, std::vector<Edge>> adjacencyList; // exemplo: {sao paulo, [target: rio, weight: 2]}

public:
    void addVertex(const std::string &vertex);

    void addEdge(const std::string &source, const std::string target, int weight);

    const std::vector<Edge> &getNeighbors(const std::string &vertex) const;
    /*
        retorna os adjacentes a ele e seu peso; entao no exemplo dado:
        (2)
 sigma ----- aura
    \
     \
    (1)
       \
        rizz

        getNeighbors("sigma") devolve:

        std::vector<Edge> resultado = {
            Edge{"aura", 2},  // aresta para aura com peso 2
            Edge{"rizz", 1}   // aresta para rizz com peso 1
        };
    */

    bool hasVertex(const std::string &vertex) const; // retorna verdadeiro ou falso pra caso exista aquele vertice dentro do grafo

    std::vector<std::string> getVertices() const; // retorna todos os vertices do grafo
};
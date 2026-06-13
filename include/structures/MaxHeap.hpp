#pragma once

#include <vector>
#include "Graph.hpp"
#include "HeapNode.hpp"

class MaxHeap
{
private:
    std::vector<HeapNode> heap;

public:
    void buildFromGraph(const Graph &graph); // função responsavel por construir o heap de max apartir do grafo
    /*
         A pontuação de cada vértice pode ser calculada pela soma
         dos pesos de suas arestas

         exemplo
         sigma --5-- aura
         sigma --3-- rizz
         sigma --2-- ohio
        score de sigma: 10

        fica em aberto como quem pegou essa parte implementara isso
    */
    HeapNode extractMax(); // retorna o "objeto" que esta no topo (com a string e a sua pontuação)

    std::vector<HeapNode> getTopK(int k) const; // retorna os k primeiros "objetos" da heap

    bool empty() const; // retorna true caso vazio
};
/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

Dijkstra.hpp

Responsável por encontrar os caminhos "mais fortes" ou mais conectados
entre dois termos de brainrot dentro do grafo.

Exemplo de uso:
Dijkstra dijkstra;
PathResult resultado = dijkstra.findShortestPath(graph, "sigma", "ohio");

Se o grafo tiver:
sigma --(2)-- aura --(1)-- ohio

O retorno esperado em PathResult será:
path: ["sigma", "aura", "ohio"]
found: true
*/

#pragma once

#include <string>
#include "../graph/Graph.hpp"
#include "../graph/PathResult.hpp"

class Dijkstra
{
public:
    /**
     * @brief Encontra o caminho de maior conectividade entre dois vértices.
     * @param graph Grafo ponderado de coocorrência.
     * @param startVertex Vértice de origem (ex: "sigma").
     * @param endVertex Vértice de destino (ex: "ohio").
     * @return PathResult Estrutura contendo o caminho e metadados do sucesso da busca.
     */
    PathResult findShortestPath(const Graph &graph, const std::string &startVertex, const std::string &endVertex);
};
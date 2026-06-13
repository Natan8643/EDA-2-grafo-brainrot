/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

com o bfs retornando os niveis poderiamos modelar o retorno como por exemplo

sigma
├─ aura
├─ rizz
└─ grindset
aura
└─ ohio

Executando bfs a partir de sigma:

Nível 0:
sigma

Nível 1:
aura
rizz
grindset

Nível 2:
ohio

*/

#pragma once

#include <vector>
#include "BFSResult.hpp"

#include "../graph/Graph.hpp"

class BFS
{
public:
    std::vector<BFSResult> traverse(const Graph &graph, const std::string &startVertex);
    /*
        retornaria um vetor contendo BFSResult exemplo partindo de sigma : {
    {"sigma", 0},
    {"aura", 1},
    {"rizz", 1},
    {"grindset", 1},
    {"ohio", 2}
}
    */
};
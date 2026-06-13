// include/graph/PathResult.hpp
#pragma once

#include <string>
#include <vector>

struct PathResult
{
    // Sequência de vértices encontrada pelo algoritmo.
    std::vector<std::string> path; // Ex: ["sigma", "aura", "ohio"]

    // Soma dos pesos do caminho encontrado.
    double totalWeight;            // Pode ser a soma dos pesos originais ou o custo acumulado

    // Indica se o caminho realmente foi encontrado.
    bool found;                    // Define se o caminho de fato existe
};
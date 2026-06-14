// include/graph/PathResult.hpp
#pragma once

#include <string>
#include <vector>

struct PathResult
{
    std::vector<std::string> path; // Ex: ["sigma", "aura", "ohio"]
    double totalWeight;            // Pode ser a soma dos pesos originais ou o custo acumulado
    bool found;                    // Define se o caminho de fato existe
};
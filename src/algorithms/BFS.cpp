#include "algorithms/BFS.hpp"
#include <queue>
#include <unordered_set>

// Implementação futura do BFS.
// TODO: deve haver alteração para utilizar a Queue proprietária, assim que ela for feita
// Além de alterar o próprio objeto, alterar também as funções para enqueue, dequeue...

std::vector<BFSResult> BFS::traverse(const Graph &graph, const std::string &startVertex){
    std::vector<BFSResult> res;
    std::queue<std::pair<std::string, int>> q;
    std::unordered_set<std::string> visited;

    if(!graph.hasVertex(startVertex)) return res;

    // Comeca pelo vertice de origem no nivel 0
    q.push({startVertex, 0});
    visited.insert(startVertex);

    while(!q.empty()){
        auto [vertex, level] = q.front();
        q.pop();

        res.push_back({vertex, level});

        // Vizinhos ainda nao visitados ganham nivel atual + 1
        for(const auto &edge: graph.getNeighbors(vertex)){
            if(visited.find(edge.target) != visited.end()) continue;

            visited.insert(edge.target);
            q.push({edge.target, level + 1});
        }
    }
    return res;
}

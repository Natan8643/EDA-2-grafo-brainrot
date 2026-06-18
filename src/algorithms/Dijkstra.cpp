#include "algorithms/Dijkstra.hpp"
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <limits>

constexpr double INF = std::numeric_limits<double>::max();

PathResult Dijkstra::findShortestPath(const Graph &graph, const std::string &startVertex, const std::string &endVertex){
    // Min-heap: top() sempre tem a menor distancia
    std::priority_queue<std::pair<double, std::string>,
        std::vector<std::pair<double, std::string>>,
        std::greater<>> heap;
    // distancia.at(v) = {melhorDist ate v, vertice anterior}
    std::unordered_map<std::string, std::pair<double, std::string>> distance;

    if(!graph.hasVertex(startVertex)) return {{}, INF, false};

    heap.push({0, startVertex});
    distance.try_emplace(startVertex, 0, "");

    while(!heap.empty()){
        auto [currDist, currVertex] = heap.top();
        heap.pop();

        // Entrada obsoleta? ja achamos um caminho melhor
        if(currDist > distance[currVertex].first) continue;

        // Primeira vez que endVertex sai da heap = menor distancia possivel
        if(currVertex == endVertex) break;

        for(const auto &edge: graph.getNeighbors(currVertex)){
            double newDist = currDist + edge.weight;

            // try_emplace: se edge.target nao existe, cria com INF e ""
            auto [it, inserted] = distance.try_emplace(edge.target, INF, "");
            auto& [bestDist, prev] = it->second;

            if(newDist < bestDist){
                bestDist = newDist;
                prev = currVertex;
                heap.push({newDist, edge.target});
            }
        }
    }

    // endVertex nunca foi visitado?
    auto it = distance.find(endVertex);
    if(it == distance.end() || it->second.first == INF){
        return {{}, INF, false};
    }

    // Reconstroi o caminho do fim ate o comeco, depois inverte
    std::vector<std::string> path;
    std::string v = endVertex;
    while(!v.empty()){
        path.push_back(v);
        v = distance[v].second;
    }
    std::reverse(path.begin(), path.end());

    return {path, distance[endVertex].first, true};
}

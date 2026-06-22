#include "algorithms/BFS.hpp"
#include "structures/Queue.hpp"
#include <unordered_set>

std::vector<BFSResult> BFS::traverse(const Graph &graph, const std::string &startVertex){
    std::vector<BFSResult> res;
    Queue<std::pair<std::string, int>> q;
    std::unordered_set<std::string> visited;

    if(!graph.hasVertex(startVertex)) return res;

    q.enqueue({startVertex, 0});
    visited.insert(startVertex);

    while(!q.empty()){
        auto [vertex, level] = q.front();
        q.dequeue();

        res.push_back({vertex, level});

        for(const auto &edge: graph.getNeighbors(vertex)){
            if(visited.find(edge.target) != visited.end()) continue;

            visited.insert(edge.target);
            q.enqueue({edge.target, level + 1});
        }
    }
    return res;
}

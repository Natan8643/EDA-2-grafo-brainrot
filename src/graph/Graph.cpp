#include "graph/Graph.hpp"

void Graph::addVertex(const std::string &vertex) {
  // se find for igual ao final -> n encontrado
  if (adjacencyList.find(vertex) == adjacencyList.end()) {
    adjacencyList[vertex] = std::vector<Edge>();
  }
}

void Graph::addEdge(const std::string &source, const std::string target,
                    int weight) {
  addVertex(source);
  addVertex(target);
  adjacencyList[source].push_back(Edge{target, weight}); // busca o source
  adjacencyList[target].push_back(Edge{source, weight});
}

const std::vector<Edge> &Graph::getNeighbors(const std::string &vertex) const {
  static const std::vector<Edge> empty;
  auto it = adjacencyList.find(vertex);

  if (it != adjacencyList.end()) {
    return it->second;
  }
  return empty;
}

bool Graph::hasVertex(const std::string &vertex) const {
  return adjacencyList.find(vertex) != adjacencyList.end();
}

std::vector<std::string> Graph::getVertices() const {
  std::vector<std::string> vertices;
  vertices.reserve(adjacencyList.size());
  for (const auto &pair : adjacencyList) {
    vertices.push_back(pair.first);
  }
  return vertices;
}
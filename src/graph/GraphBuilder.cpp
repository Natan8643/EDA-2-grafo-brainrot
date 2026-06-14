#include "graph/GraphBuilder.hpp"
#include <algorithm>
#include <unordered_set>

std::string GraphBuilder::makeEdgeKey(const std::string &wordA,
                                      const std::string &wordB) const {
  if (wordA < wordB) {
    return wordA + "|" + wordB;
  }
  return wordB + "|" + wordA;
}

void GraphBuilder::addDocument(const std::vector<Token> &tokens) {
  std::unordered_set<std::string> uniqueLemmas; // lemmas sem repetição?

  // sem rep
  for (const auto &token : tokens) {
    if (!token.lemma.empty()) {
      uniqueLemmas.insert(token.lemma);
    }
  }

  std::vector<std::string> words(uniqueLemmas.begin(), uniqueLemmas.end());

  // ordem alf
  std::sort(words.begin(), words.end());

  /*ex: ["aura", "sigma", "rizz"]
    tem que ficar: aura-sigma, aura-rizz, sigma-rizz que seria a combinação com
    todos

  */
  for (size_t i = 0; i < words.size(); i++) {
    for (size_t j = i + 1; j < words.size(); j++) {
      std::string key = makeEdgeKey(words[i], words[j]);
      edgeCounter[key]++; // seria como mapa["aura|ego"]++
    }
  }
}

Graph GraphBuilder::build() {
  Graph graph;
  // pra cada caso dentro de edgeCounter separa o from e to e adiciona ao
  // vertice de um grafo (adicona o peso)
  for (const auto &[key, weight] : edgeCounter) {
    auto separator = key.find('|');
    std::string from = key.substr(0, separator);
    std::string to = key.substr(separator + 1);

    graph.addVertex(from);
    graph.addVertex(to);
    graph.addEdge(from, to, weight);
  }
}
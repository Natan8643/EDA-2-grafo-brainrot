#pragma once

#include "Graph.hpp"
#include <string>

class ReportGenerator {
public:
  ReportGenerator() = default;

  // Auxiliares internos para formatar cada seção do relatório
  void printStats(const Graph &g) const;
  void printTopTerms(const Graph &g, int k) const;
  void printBFSDistances(const Graph &g, const std::string &rootTerm) const;
  void printShortestPath(const Graph &g, const std::string &startWord, const std::string &endWord) const;
  void exportGraphToJson(const Graph &g, const std::string &filepath) const;

  // Método público principal que orquestra todo o fluxo de relatório e exportação
  void generateSummary(const Graph &g, 
                       const std::string &startWord, 
                       const std::string &endWord, 
                       const std::string &jsonPath = "data/graph.json") const;
};
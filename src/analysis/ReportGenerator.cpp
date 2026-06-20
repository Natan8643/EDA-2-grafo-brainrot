#include "analysis/ReportGenerator.hpp"
#include "structures/MaxHeap.hpp"
#include "algorithms/BFS.hpp"
#include "algorithms/Dijkstra.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

void ReportGenerator::printStats(const Graph &g) const {
  std::cout << "\n[1] ESTATÍSTICAS DO GRAFO" << std::endl;
  auto vertices = g.getVertices();
  std::cout << "  - Total de Vértices (Lemas Únicos): " << vertices.size() << std::endl;
  
  int totalEdges = 0;
  for (const auto& v : vertices) {
    totalEdges += g.getNeighbors(v).size();
  }
  std::cout << "  - Total de Arestas Ponderadas: " << (totalEdges / 2) << std::endl;
}

void ReportGenerator::printTopTerms(const Graph &g, int k) const {
  std::cout << "\n[2] TERMOS MAIS RELEVANTES (MaxHeap - Top " << k << ")" << std::endl;
  MaxHeap heap;
  heap.buildFromGraph(g);
  auto topK = heap.getTopK(k);
  for (size_t i = 0; i < topK.size(); ++i) {
    std::cout << "  " << (i + 1) << ". \"" << topK[i].word << "\" (Score de Conexão: " << topK[i].score << ")" << std::endl;
  }
}

void ReportGenerator::printBFSDistances(const Graph &g, const std::string &rootTerm) const {
  if (g.hasVertex(rootTerm)) {
    std::cout << "\n[3] DISTÂNCIA DE TERMOS A PARTIR DE \"" << rootTerm << "\" (BFS)" << std::endl;
    BFS bfs;
    auto bfsResult = bfs.traverse(g, rootTerm);
    // Agrupa e mostra termos até nível 2
    for (int lvl = 1; lvl <= 2; ++lvl) {
      std::cout << "  - Nível " << lvl << " (Conexão direta ou a " << lvl << " saltos):" << std::endl;
      bool found = false;
      for (const auto& node : bfsResult) {
        if (node.level == lvl) {
          std::cout << "      \"" << node.word << "\"";
          found = true;
        }
      }
      if (!found) std::cout << "      (Nenhum encontrado)";
      std::cout << std::endl;
    }
  }
}

void ReportGenerator::printShortestPath(const Graph &g, const std::string &startWord, const std::string &endWord) const {
  if (g.hasVertex(startWord) && g.hasVertex(endWord)) {
    std::cout << "\n[4] MENOR CAMINHO DE ASSOCIAÇÃO (Dijkstra)" << std::endl;
    std::cout << "  Calculando caminho forte de \"" << startWord << "\" até \"" << endWord << "\"..." << std::endl;
    
    Dijkstra dijkstra;
    auto pathRes = dijkstra.findShortestPath(g, startWord, endWord);
    if (pathRes.found) {
      std::cout << "  - Caminho encontrado: ";
      for (size_t i = 0; i < pathRes.path.size(); ++i) {
        std::cout << "\"" << pathRes.path[i] << "\"";
        if (i + 1 < pathRes.path.size()) std::cout << " -> ";
      }
      std::cout << "\n  - Peso total acumulado: " << pathRes.totalWeight << std::endl;
    } else {
      std::cout << "  - Nenhum caminho de conexão encontrado entre esses termos." << std::endl;
    }
  } else {
    std::cout << "\n[4] MENOR CAMINHO DE ASSOCIAÇÃO (Dijkstra)" << std::endl;
    std::cout << "  - Erro: Um ou ambos os termos (\"" << startWord << "\", \"" << endWord << "\") não existem no grafo." << std::endl;
  }
}

void ReportGenerator::exportGraphToJson(const Graph &g, const std::string &filepath) const {
  std::ofstream out(filepath);
  if (!out.is_open()) return;

  out << "{\n  \"nodes\": [\n";
  auto vertices = g.getVertices();
  for (size_t i = 0; i < vertices.size(); ++i) {
    out << "    {\"id\": \"" << vertices[i] << "\", \"label\": \"" << vertices[i] << "\"}";
    if (i + 1 < vertices.size()) out << ",";
    out << "\n";
  }
  out << "  ],\n  \"edges\": [\n";

  bool first = true;
  for (const auto& u : vertices) {
    for (const auto& edge : g.getNeighbors(u)) {
      if (u < edge.target) { // Evita duplicar arestas em grafo não direcionado
        if (!first) out << ",\n";
        out << "    {\"from\": \"" << u << "\", \"to\": \"" << edge.target << "\", \"value\": " << edge.weight << "}";
        first = false;
      }
    }
  }
  out << "\n  ]\n}\n";
  out.close();
}

void ReportGenerator::generateSummary(const Graph &g, 
                                     const std::string &startWord, 
                                     const std::string &endWord, 
                                     const std::string &jsonPath) const {
  std::cout << "\n=============================================" << std::endl;
  std::cout << "      ANALISADOR DE COOCORRÊNCIA BRAINROT     " << std::endl;
  std::cout << "=============================================" << std::endl;

  printStats(g);
  printTopTerms(g, 5);

  // Determina o termo mais relevante para rodar o BFS dinamicamente
  MaxHeap heap;
  heap.buildFromGraph(g);
  if (!heap.empty()) {
    printBFSDistances(g, heap.getTopK(1)[0].word);
  }

  printShortestPath(g, startWord, endWord);

  std::cout << "\n[5] EXPORTAÇÃO DOS DADOS" << std::endl;
  exportGraphToJson(g, jsonPath);
  std::cout << "  - Arquivo '" << jsonPath << "' exportado com sucesso." << std::endl;
  std::cout << "  - Digite: python3 data/visualize.py para abrir o grafo interativo no navegador!" << std::endl;
  std::cout << "=============================================\n" << std::endl;
}

#include "algorithms/BFS.hpp"
#include "algorithms/Dijkstra.hpp"
#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"
#include "nlp/SpacyProcessor.hpp"
#include "parser/CorpusManager.hpp"
#include "parser/FileLoader.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdio.h>

#define CORPUSPATH "data/corpus"

void printa(std::vector<std::string> text) {
  for (size_t i = 0; i < text.size(); i++) {
    std::cout << text.at(i) << std::endl;
  }
}

int main() {
  // Limpa/sobrescreve o relatório para iniciar do zero
  std::ofstream cleanReport("data/nlp_report.txt");
  if (cleanReport.is_open()) {
    cleanReport << "=== RELATÓRIO DE PROCESSAMENTO NLP E GRAFO ===\n\n";
  }
  cleanReport.close();

  CorpusManager maneger(CORPUSPATH);
  FileLoader loader;

  std::vector<std::string> docs = maneger.getDocumentPaths();

  std::vector<std::string> text(docs.size());

  for (size_t i = 0; i < docs.size(); i++) {
    text[i] = loader.loadFile(docs.at(i));
  }

  SpacyProcessor nlp;
  GraphBuilder builder;

  for (size_t i = 0; i < text.size(); i++) {
    std::vector<Token> tokens = nlp.process(text.at(i), docs.at(i));

    builder.addDocument(tokens);
  }

  Graph g = builder.build();

  return 0;
}
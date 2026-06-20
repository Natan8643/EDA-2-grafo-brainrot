#include "analysis/ReportGenerator.hpp"
#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"
#include "nlp/SpacyProcessor.hpp"
#include "parser/CorpusManager.hpp"
#include "parser/FileLoader.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#define CORPUSPATH "data/corpus"

void printa(std::vector<std::string> text) {
  for (size_t i = 0; i < text.size(); i++) {
    std::cout << text.at(i) << std::endl;
  }
}

int main() {
  CorpusManager maneger(CORPUSPATH);
  FileLoader loader;

  std::vector<std::string> docs = maneger.getDocumentPaths();

  std::vector<std::string> text(docs.size());

  for (size_t i = 0; i < docs.size(); i++) {
    text[i] = loader.loadFile(docs.at(i));
  }

  SpacyProcessor nlp;
  GraphBuilder builder;

  // estava gerando a relação de todos com todos (clique)
  /*problema: cada arquivo estava sendo tratado como uma frase apenas ent se
  tinha duas frases "Os caras sigma têm aura." "os cara tomaram fanum tax"

  ele mostrava "tomar" e "sigma" ligados junto quando na verdade eles n tem
  caminho direto

        correção: pegar linha por linha e jogar no nlp
        */

  for (size_t i = 0; i < text.size(); i++) {
    std::stringstream ss(text.at(i));
    std::string line;
    while (std::getline(ss, line)) {
      // Ignora linhas vazias
      if (line.empty() ||
          line.find_first_not_of(" \t\r\n") == std::string::npos) {
        continue;
      }
      std::vector<Token> tokens = nlp.process(line);
      builder.addDocument(tokens);
    }
  }

  Graph g = builder.build();

  // Solicita termos interativamente para remover o hardcode do Dijkstra
  std::string startWord, endWord;
  std::cout << "\n>>> Configuração do Dijkstra <<<" << std::endl;
  std::cout << "Digite o termo de origem (ex: cara): ";
  std::cin >> startWord;
  std::cout << "Digite o termo de destino (ex: rizz): ";
  std::cin >> endWord;

  // Orquestra a geração do relatório através do ReportGenerator
  ReportGenerator reporter;
  reporter.generateSummary(g, startWord, endWord);

  return 0;
}
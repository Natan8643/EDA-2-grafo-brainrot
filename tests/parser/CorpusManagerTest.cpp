#include <iostream>
#include "parser/CorpusManager.hpp"
#include "parser/FileLoader.hpp"

int main() {
    CorpusManager manager("data/corpus");
    std::vector<std::string> caminhos = manager.getDocumentPaths();

    for (const auto &caminho : caminhos){
        std::string conteudo = FileLoader::loadFile(caminho);
        std::cout <<  conteudo <<  std::endl;
    }

    return 0;
}
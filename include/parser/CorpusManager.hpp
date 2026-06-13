/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

CorpusManager.hpp tem a finalidade de entrar no data, encontrar os arquivos que vao ser inseridos e devolver um vetor de caminhos pra esses arquivos
ex:

    CorpusManager manager("./data/corpus")

    saida:

    {
    "arquivo1.txt",
    "arquivo2.txt"
    }
*/

#pragma once

#include <string>
#include <vector>

class CorpusManager
{
private:
    std::string corpusPath;

public:
    explicit CorpusManager(const std::string &corpusPath);

    std::vector<std::string> getDocumentPaths() const;
};
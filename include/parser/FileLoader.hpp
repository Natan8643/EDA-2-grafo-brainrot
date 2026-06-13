/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

FileLoader.hpp tem a finalidade de ler um arquivo e devolver conteúdo em uma string

*/

#pragma once

#include <string>

class FileLoader
{
public:
    static std::string loadFile(const std::string &filePath);
};
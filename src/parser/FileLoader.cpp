#include "parser/FileLoader.hpp"
#include <fstream>
#include <sstream>

// Método estático que abre um ficheiro e extrai todo o seu texto bruto
std::string FileLoader::loadFile(const std::string &filePath) 
{
    // Abre o fluxo de entrada para leitura do ficheiro
    std::ifstream file(filePath);

    // Validação essencial: se o ficheiro não abrir (caminho errado, falta de permissão), devolve string vazia
    if (!file.is_open()) {
        return "";
    }

    // Utiliza um fluxo de strings na memória RAM para ler o buffer de forma muito eficiente
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Devolve o conteúdo acumulado em formato de std::string
    return buffer.str();
}
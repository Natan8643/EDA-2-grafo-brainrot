#include "parser/CorpusManager.hpp"
#include <filesystem>

CorpusManager::CorpusManager(const std::string &corpusPath) 
    : corpusPath(corpusPath) 
{
}

std::vector<std::string> CorpusManager::getDocumentPaths() const 
{
    std::vector<std::string> paths;

    if (!std::filesystem::exists(corpusPath) || !std::filesystem::is_directory(corpusPath)) {
        return paths;
    }

    for (const auto &entry : std::filesystem::directory_iterator(corpusPath)) {
        if (std::filesystem::is_regular_file(entry)){
            paths.push_back(entry.path().string());
        }
    }

    return paths;
}
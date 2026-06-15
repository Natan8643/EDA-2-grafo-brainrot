#pragma once
// Placeholder for Config

#include <filesystem>
#include <string>

namespace brainrot::utils
{

    struct Config
    {
        std::filesystem::path corpusDirectory{"data/corpus"};
        std::filesystem::path stopwordsFile{"data/stopWords.txt"};
        std::filesystem::path brainrotVocabularyFile{"data/brainrotVocab.txt"};
        std::string spacyModel{"pt_core_news_sm"};
        std::string applicationName{"BrainrotAnalyzer"};
    };

} // namespace brainrot::utils
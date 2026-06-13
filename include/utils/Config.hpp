#pragma once
// Placeholder for Config

#include <filesystem>
#include <string>

namespace brainrot::utils
{

    struct Config
    {
        std::filesystem::path corpusDirectory{"data/corpus"};
        std::filesystem::path stopwordsFile{"data/stopwords.txt"};
        std::string applicationName{"BrainrotAnalyzer"};
    };

} // namespace brainrot::utils
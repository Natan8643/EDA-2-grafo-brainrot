#pragma once

#include "nlp/Token.hpp"
#include "utils/Config.hpp"

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

class SpacyProcessor
{
private:
	brainrot::utils::Config config;
	std::unordered_set<std::string> stopwords;
	std::unordered_set<std::string> brainrotVocabulary;

	std::unordered_set<std::string> loadWordSet(const std::filesystem::path &filePath) const;
	std::string normalize(const std::string &text) const;
	bool isPunctuationOnly(const std::string &tokenText) const;

public:
	explicit SpacyProcessor(brainrot::utils::Config config = {});

	// Processa um documento bruto e retorna os tokens prontos para o grafo.
	std::vector<Token> process(const std::string &rawText) const;

	bool isBrainrotLemma(const std::string &lemma) const;
};

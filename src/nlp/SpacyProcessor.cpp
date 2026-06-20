/*
    o brainrotVocab.txt ta sendo usado apenas quando o isBrainrotLemma era
   chamado, é mais antes utilizar ele pra normalizar os brainrot pq tava saindo
   por exemplo aura, auro e auras ()
*/

#include "nlp/SpacyProcessor.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <utility>

#if defined(BRAINROT_HAS_SPACY_CPP) && BRAINROT_HAS_SPACY_CPP
#include <spacy/spacy>
#endif

SpacyProcessor::SpacyProcessor(brainrot::utils::Config config)
    : config(std::move(config)),
      stopwords(loadWordSet(this->config.stopwordsFile)),
      brainrotVocabulary(loadWordSet(this->config.brainrotVocabularyFile)) {}

std::vector<Token> SpacyProcessor::process(const std::string &rawText) const {
  std::vector<Token> tokens;

#if defined(BRAINROT_HAS_SPACY_CPP) && BRAINROT_HAS_SPACY_CPP
  try {
    static Spacy::Spacy spacy;
    auto nlp = spacy.load(config.spacyModel);
    auto doc = nlp.parse(rawText);

    for (const auto &spacyToken : doc.tokens()) {
      const std::string original = spacyToken.text();
      const std::string lemma = spacyToken.lemma_();
      std::string normalized = normalize(lemma.empty() ? original : lemma);

      // Se a palavra original ou o lema corresponder/começar com um termo de
      // brainrot, forçamos o uso do termo canônico definido no vocabulário.
      for (const auto &vocabWord : brainrotVocabulary) {
        if (normalized == vocabWord || normalize(original) == vocabWord ||
            normalize(original).find(vocabWord) == 0) {
          normalized = vocabWord;
          break;
        }
      }

      if (normalized.empty() || isPunctuationOnly(normalized)) {
        continue;
      }

      if (stopwords.find(normalized) != stopwords.end()) {
        continue;
      }

      tokens.push_back(Token{original, normalized});
    }

    return tokens;
  } catch (...) {
    // Falha no spaCy nativo, usando modo Fallback
  }
#endif

  // Fallback para ambientes sem spacy-cpp instalado ou sem modelo carregável.
  std::istringstream iss(rawText);
  std::string word;

  while (iss >> word) {
    std::string normalized = normalize(word);

    // Se a palavra corresponder/começar com um termo de brainrot,
    // forçamos o uso do termo canônico definido no vocabulário.
    for (const auto &vocabWord : brainrotVocabulary) {
      if (normalized == vocabWord || normalize(word).find(vocabWord) == 0) {
        normalized = vocabWord;
        break;
      }
    }
    if (normalized.empty() || isPunctuationOnly(normalized)) {
      continue;
    }

    if (stopwords.find(normalized) != stopwords.end()) {
      continue;
    }

    tokens.push_back(Token{word, normalized});
  }

  return tokens;
}

bool SpacyProcessor::isBrainrotLemma(const std::string &lemma) const {
  const std::string normalized = normalize(lemma);
  return !normalized.empty() &&
         brainrotVocabulary.find(normalized) != brainrotVocabulary.end();
}

std::unordered_set<std::string>
SpacyProcessor::loadWordSet(const std::filesystem::path &filePath) const {
  std::unordered_set<std::string> words;
  std::ifstream file(filePath);
  if (!file.is_open()) {
    return words;
  }

  std::string line;
  while (std::getline(file, line)) {
    const std::string normalized = normalize(line);
    if (normalized.empty()) {
      continue;
    }

    // Permite comentarios de linha nos arquivos de vocabulario.
    if (normalized[0] == '#') {
      continue;
    }

    words.insert(normalized);
  }

  return words;
}

std::string SpacyProcessor::normalize(const std::string &text) const {
  std::string out;
  out.reserve(text.size());

  for (char ch : text) {
    const unsigned char uch = static_cast<unsigned char>(ch);
    if (std::ispunct(uch)) {
      continue;
    }

    out.push_back(static_cast<char>(std::tolower(uch)));
  }

  const auto beginIt =
      std::find_if_not(out.begin(), out.end(),
                       [](unsigned char c) { return std::isspace(c) != 0; });

  if (beginIt == out.end()) {
    return {};
  }

  const auto rbeginIt =
      std::find_if_not(out.rbegin(), out.rend(),
                       [](unsigned char c) { return std::isspace(c) != 0; });

  return std::string(beginIt, rbeginIt.base());
}

bool SpacyProcessor::isPunctuationOnly(const std::string &tokenText) const {
  if (tokenText.empty()) {
    return true;
  }

  return std::all_of(tokenText.begin(), tokenText.end(),
                     [](unsigned char c) { return std::ispunct(c) != 0; });
}

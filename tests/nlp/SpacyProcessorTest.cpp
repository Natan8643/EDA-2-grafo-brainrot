#include <iostream>

#include "nlp/SpacyProcessor.hpp"

int main() {
#if defined(BRAINROT_HAS_SPACY_CPP) && BRAINROT_HAS_SPACY_CPP
  std::cout << "[INFO] Compilado COM suporte nativo ao spaCy (spacy-cpp)."
            << std::endl;
#else
  std::cout << "[INFO] Compilado em modo FALLBACK (sem suporte ao spaCy)."
            << std::endl;
#endif

  SpacyProcessor processor;
  const std::string sample =
      "os jogadores farmaram muita aura e tinham muito rizz";
  const auto tokens = processor.process(sample);

  std::cout << "Tokens processados:" << std::endl;
  for (const auto &token : tokens) {
    std::cout << "- original: " << token.original
              << " | lemma: " << token.lemma;
    if (processor.isBrainrotLemma(token.lemma)) {
      std::cout << " | brainrot: sim";
    }
    std::cout << std::endl;
  }

  return 0;
}

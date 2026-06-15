#include <iostream>

#include "nlp/SpacyProcessor.hpp"

int main()
{
    SpacyProcessor processor;
    const std::string sample = "Os caras sigma tem muito rizz e aura!";
    const auto tokens = processor.process(sample);

    std::cout << "Tokens processados:" << std::endl;
    for (const auto &token : tokens)
    {
        std::cout << "- original: " << token.original << " | lemma: " << token.lemma;
        if (processor.isBrainrotLemma(token.lemma))
        {
            std::cout << " | brainrot: sim";
        }
        std::cout << std::endl;
    }

    return 0;
}

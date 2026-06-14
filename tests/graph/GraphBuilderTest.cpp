#include "GraphBuilder.hpp"
#include <iostream>

int main() {

  GraphBuilder builder;

  Token t1;
  t1.lemma = "cara";
  t1.original = "caras";

  Token t2;
  t2.lemma = "sigma";
  t2.original = "sigma";

  std::vector<Token> tokens = {t1, t2};

  builder.addDocument(tokens);

  // std::cout << "a" << std::endl;
  // std::cout << "GraphBuilder sandbox" << std::endl;

  return 0;
}

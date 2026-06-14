/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

GraphBuilder.hpp

Responsável por transformar a saída do SpacyProcessor
(vetor de Tokens) em um Grafo de Coocorrência.

Fluxo esperado:

1. Receber um documento já processado pelo SpacyProcessor:
   Ex:
       {"sigma", "aura", "sigma", "rizz"}

2. Utilizar o lemma dos Tokens e remover duplicatas
   do documento:
       {"sigma", "aura", "rizz"}

3. Gerar todas as coocorrências do documento:
       sigma - aura
       sigma - rizz
       aura  - rizz

4. Armazenar/atualizar a frequência de cada aresta
   utilizando edgeCounter.

   Exemplo:
       edgeCounter["aura|sigma"]++;
       edgeCounter["rizz|sigma"]++;
       edgeCounter["aura|rizz"]++;

5. O peso da aresta representa a quantidade de
   documentos em que aquela associação apareceu.

6. O método build() percorre o edgeCounter e cria
   o objeto Graph final com vértices, arestas e pesos.

Obs
- Repetições da mesma palavra dentro do mesmo documento
  não aumentam o peso da aresta.
- A chave gerada por makeEdgeKey() deve ser normalizada
  para evitar duplicidade:
      "sigma|aura"
      "aura|sigma"

  devem representar a mesma aresta.
*/
// Chave da aresta normalizada.
// Exemplo:
// "aura|sigma" -> 3
//
// Significa que aura e sigma apareceram juntos
// em 3 documentos diferentes.
#pragma once

#include "../nlp/Token.hpp"
#include "Graph.hpp"
#include <unordered_map>
#include <vector>

class GraphBuilder {
private:
  std::unordered_map<std::string, int> edgeCounter;

  std::string makeEdgeKey(const std::string &wordA,
                          const std::string &wordB) const;

public:
  void addDocument(const std::vector<Token> &tokens);

  Graph build();
};

/*
    impl?

class GraphBuilder
{
private:
    std::unordered_map<std::string, int> edgeCounter;

    std::string makeEdgeKey(const std::string& wordA, const std::string& wordB)
const
    {
        if (wordA < wordB) {
            return wordA + "|" + wordB;
        }
        return wordB + "|" + wordA;
    }

public:
    void addDocument(const std::vector<Token>& tokens)
    {
        std::unordered_set<std::string> uniqueLemmas;

        for (const auto& token : tokens) {
            if (!token.lemma.empty()) {
                uniqueLemmas.insert(token.lemma);
            }
        }

        std::vector<std::string> words(uniqueLemmas.begin(),
uniqueLemmas.end()); std::sort(words.begin(), words.end());

        for (size_t i = 0; i < words.size(); ++i) {
            for (size_t j = i + 1; j < words.size(); ++j) {
                std::string key = makeEdgeKey(words[i], words[j]);
                edgeCounter[key]++;
            }
        }
    }

    Graph build()
    {
        Graph graph;

        for (const auto& [key, weight] : edgeCounter) {
            auto separator = key.find('|');
            std::string from = key.substr(0, separator);
            std::string to = key.substr(separator + 1);

            graph.addVertex(from);
            graph.addVertex(to);
            graph.addEdge(from, to, weight);
        }

        return graph;
    }
};
*/
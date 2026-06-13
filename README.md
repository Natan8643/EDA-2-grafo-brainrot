# Especificação do Fluxo de Dados — Brainrot Analyzer

Este documento descreve o ciclo de vida dos dados dentro do sistema, detalhando a entrada e a saída de cada módulo para guiar a implementação correta das classes.

---

## Cenário de Exemplo

Para ilustrar o fluxo, utilizaremos um corpus composto por 2 documentos textuais salvos na pasta `data/corpus/`:

- **doc1.txt:** `"Os caras sigma têm aura."`
- **doc2.txt:** `"Aquele sigma tem muito rizz, que aura!"`

---

## Módulo 1: Parser (Leitura de Arquivos)

### 1. CorpusManager

**Responsabilidade:** Varrer o diretório `data/corpus/` e listar todos os caminhos de arquivos válidos.

- **Entrada:** Caminho do diretório (`"data/corpus"`)
- **Saída** (`std::vector<std::string>`):

```cpp
{ "data/corpus/doc1.txt", "data/corpus/doc2.txt" }
```

### 2. FileLoader

**Responsabilidade:** Abrir cada arquivo individualmente e extrair seu texto bruto em formato de string.

- **Entrada:** Caminho obtido pelo `CorpusManager` (ex: `"data/corpus/doc1.txt"`)
- **Saída** (`std::string`):
  - Para `doc1.txt`: `"Os caras sigma têm aura."`
  - Para `doc2.txt`: `"Aquele sigma tem muito rizz, que aura!"`

---

## Módulo 2: NLP (Processamento de Linguagem Natural)

### SpacyProcessor

**Responsabilidade:** Receber o texto bruto de um documento, aplicar tokenização, remoção de pontuação, filtragem de stopwords e lematização utilizando o wrapper `spacy-cpp`.

- **Entrada** (`std::string`): Conteúdo textual de um arquivo.
- **Saída por Documento** (`std::vector<Token>`):

Após processar `doc1.txt`:

- Filtrados: `"Os"`, `"têm"` (stopwords) e `"."` (pontuação)
- Lematizado: `"caras"` → `"cara"`

```cpp
{
    Token{ original: "caras", lemma: "cara" },
    Token{ original: "sigma", lemma: "sigma" },
    Token{ original: "aura",  lemma: "aura" }
}
```

Após processar `doc2.txt`:

- Filtrados: `"Aquele"`, `"tem"`, `"muito"`, `"que"` (stopwords) e `"!"` (pontuação)

```cpp
{
    Token{ original: "sigma", lemma: "sigma" },
    Token{ original: "rizz",  lemma: "rizz" },
    Token{ original: "aura",  lemma: "aura" }
}
```

---

## Módulo 3: Graph (Construção da Rede)

### GraphBuilder

**Responsabilidade:** Receber o vetor de tokens de cada documento, remover duplicatas internas de lemas e gerar os pares de coocorrência, alimentando uma tabela hash para computar os pesos baseados em presença por documento.

**Passo Interno 1: Remoção de duplicatas por documento**

- `doc1`: `["cara", "sigma", "aura"]`
- `doc2`: `["sigma", "rizz", "aura"]`

**Passo Interno 2: Geração de chaves normalizadas pelo `makeEdgeKey()`**

As palavras são ordenadas alfabeticamente para evitar duplicidade na contagem do grafo não direcionado:

- Coocorrências de `doc1`: `"aura|cara"`, `"cara|sigma"`, `"aura|sigma"`
- Coocorrências de `doc2`: `"rizz|sigma"`, `"aura|sigma"`, `"aura|rizz"`

**Passo Interno 3: Estado final do `edgeCounter`**

| Aresta        | Peso |
| ------------- | ---- |
| `aura\|sigma` | 2    |
| `aura\|cara`  | 1    |
| `cara\|sigma` | 1    |
| `rizz\|sigma` | 1    |
| `aura\|rizz`  | 1    |

> `edgeCounter["aura|sigma"] = 2` — apareceu em ambos os documentos.

**Saída: O Objeto Graph Final**

Quando o método `build()` é chamado, ele monta a lista de adjacência:

```
        (1)        (1)
cara -------- sigma -------- rizz
  \            |            /
   \        (2)|           /
    \          |          /
     \-------- aura -----/
          (1)       (1)
```

```
getNeighbors("sigma") → [Edge{"aura", 2}, Edge{"cara", 1}, Edge{"rizz", 1}]
getNeighbors("aura")  → [Edge{"sigma", 2}, Edge{"cara", 1}, Edge{"rizz", 1}]
```

---

## Módulo 4: Algorithms & Structures (Análise)

### 1. MaxHeap (Identificação de Relevância)

**Responsabilidade:** Calcular o score de cada vértice (soma dos pesos das suas arestas) e ordená-los de forma decrescente.

**Cálculo dos Scores no exemplo:**

| Vértice | Cálculo                         | Score |
| ------- | ------------------------------- | ----- |
| sigma   | 2 (aura) + 1 (cara) + 1 (rizz)  | 4     |
| aura    | 2 (sigma) + 1 (cara) + 1 (rizz) | 4     |
| rizz    | 1 (sigma) + 1 (aura)            | 2     |
| cara    | 1 (sigma) + 1 (aura)            | 2     |

**Saída da Heap** (`extractMax()` sucessivos):

```
HeapNode{"sigma", 4}
HeapNode{"aura",  4}
HeapNode{"rizz",  2}
HeapNode{"cara",  2}
```

---

### 2. BFS (Varredura por Proximidade)

**Responsabilidade:** Explorar o grafo por níveis a partir de uma palavra raiz de alta relevância, utilizando a fila própria (`Queue`).

- **Entrada:** `graph`, `startVertex = "sigma"`
- **Saída** (`std::vector<BFSResult>`):

```cpp
{
    {"sigma", 0}, // Raiz
    {"aura",  1}, // Vizinhos diretos de sigma
    {"cara",  1},
    {"rizz",  1}
}
```

---

### 3. Dijkstra (Caminhos Fortes)

**Responsabilidade:** Encontrar a menor distância com base no custo inverso da coocorrência (`1 / peso`) para mapear as conexões conceituais mais robustas.

- **Entrada:** `graph`, `startVertex = "cara"`, `endVertex = "rizz"`

**Análise de Caminhos:**

| Caminho                             | Custo                     |
| ----------------------------------- | ------------------------- |
| Direto `cara → rizz`                | Não existe aresta         |
| Via sigma: `cara → sigma → rizz`    | 1/1 + 1/1 = **2.0**       |
| Via aura: `cara → aura → rizz`      | 1/1 + 1/1 = **2.0**       |
| Misto: `cara → aura → sigma → rizz` | 1.0 + 0.5 + 1.0 = **2.5** |

**Saída** (`PathResult`):

```cpp
PathResult {
    path: ["cara", "sigma", "rizz"], // Ou via aura, ambos têm custo mínimo 2.0
    totalWeight: 2,                  // Soma dos pesos originais das arestas percorridas
    totalCost: 2.0,                  // Custo inverso acumulado
    found: true
}
```

---

## Módulo 5: Analysis (Consolidação do Relatório)

### ReportGenerator

**Responsabilidade:** Coletar os dados finais calculados por todos os módulos anteriores e formatar um relatório legível que responda às hipóteses da rede de termos brainrot.

- **Entrada:** Resultados agregados da `MaxHeap`, `BFS` e `Dijkstra`.
- **Saída:** Arquivo de texto gerado ou saída formatada no terminal detalhando o comportamento linguístico do corpus estruturado.

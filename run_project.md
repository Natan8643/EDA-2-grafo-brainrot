## Configuração Inicial (Gerar pasta de Build)

Precisa do CMake e o compilador do c++ instalado:

```
sudo apt update
sudo apt install cmake build-essential
```

Também é necessario a instalação do wrapper do spacy pra c++ e ter o python instalado:
(assumindo que o python está instalado e que o gerenciador do python também está instalado)
```
python3 -m pip install spacy --break-system-packages
```
```
python3 -m spacy download pt_core_news_sm --break-system-packages
```

CMake precisa mapear o seu sistema:

Bash

```
cmake -S . -B build
```

esse comando cria a pasta **build/** e joga lá dentro os scripts que o compilador real do seu sistema vai usar para montar o binário.


## Como usar as Sandboxes (Ambiente de Teste Livre)

Para evitar que todo mundo mexe no `main.cpp` ao mesmo tempo e gere conflitos no Git, cada módulo ganhou um Sandbox isolado dentro da pasta **tests/**.

Cada um desses arquivos possui o seu próprio `int main()`. Você pode abrir o arquivo do seu módulo, escrever códigos rápidos para ver se sua lógica funciona e dar `std::cout` à vontade para debugar.

Para compilar o seu código fonte e **executar direto** o seu arquivo de teste, use o comando correspondente:

- **Módulo:** Parser **Arquivo para alterar/testar:** `tests/parser/CorpusManagerTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_parser`

- **Módulo:** NLP **Arquivo para alterar/testar:** `tests/nlp/SpacyProcessorTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_nlp`

- **Módulo:** Graph **Arquivo para alterar/testar:** `tests/graph/GraphTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_graph`

`cmake --build build --target run_graph_builder`

- **Módulo:** Algorithms **Arquivo para alterar/testar:** `tests/algorithms/BFSTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_algorithms`

- **Módulo:** Structures **Arquivo para alterar/testar:** `tests/structures/MaxHeapTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_structures`

- **Módulo:** Analysis **Arquivo para alterar/testar:** `tests/analysis/ReportGeneratorTest.cpp`
- **Comando para rodar:** `cmake --build build --target run_analysis`

_O CMake é inteligente:_ Ao rodar `run_graph`, por exemplo, ele detecta se você alterou algo em `src/graph/Graph.cpp`, compila as alterações automaticamente e já executa o `GraphTest.cpp` na sua tela com um único comando.

## Compilar Apenas os Módulos (Sem Executar)

Se você quiser apenas checar se o código do seu módulo está compilando sem erros de sintaxe (sem rodar nenhum print na tela), você pode dar build apenas na biblioteca estática dele:

Bash

```
cmake --build build --target brainrot_parser
cmake --build build --target brainrot_nlp
cmake --build build --target brainrot_graph
cmake --build build --target brainrot_algorithms
cmake --build build --target brainrot_structures
cmake --build build --target brainrot_analysis
cmake --build build --target brainrot_utils
```

## Rodar o Projeto Inteiro Integrado

Quando todos os módulos estiverem prontos e conversando entre si, o fluxo oficial completo da aplicação rodará a partir do `main.cpp` da raiz. Para disparar a execução completa do sistema:

Bash

```
cmake --build build --target run_project
```

Caso queiram rodar todos os testes de parquinho de todo mundo e o projeto integrado em sequência:

Bash

```
cmake --build build --target run_all
```

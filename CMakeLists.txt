cmake_minimum_required(VERSION 3.16)

project(BrainrotAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

set(BRAINROT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BRAINROT_INCLUDE_SUBDIRS
    ${BRAINROT_INCLUDE_DIR}/algorithms
    ${BRAINROT_INCLUDE_DIR}/analysis
    ${BRAINROT_INCLUDE_DIR}/graph
    ${BRAINROT_INCLUDE_DIR}/nlp
    ${BRAINROT_INCLUDE_DIR}/parser
    ${BRAINROT_INCLUDE_DIR}/structures
    ${BRAINROT_INCLUDE_DIR}/utils
)

# Função para empacotar o código fonte de cada módulo em uma biblioteca estática
function(brainrot_add_library target_name)
    add_library(${target_name} STATIC ${ARGN})
    target_include_directories(${target_name} PUBLIC ${BRAINROT_INCLUDE_DIR} ${BRAINROT_INCLUDE_SUBDIRS})
    target_compile_features(${target_name} PUBLIC cxx_std_17)
endfunction()

# Função que transforma a pasta tests/ em um parquinho (Sandbox) isolado para a galera rodar código
function(brainrot_add_runner run_target_name module_library test_source_file)

    # 1. Cria o executável usando o arquivo real que a galera vai mexer na pasta tests/
    set(executable_name "exec_${run_target_name}")
    add_executable(${executable_name} ${test_source_file})
    
    # 2. Linka com a biblioteca do módulo correspondente e com a utils (caso usem Logger)
    target_link_libraries(${executable_name} PRIVATE ${module_library} brainrot_utils)

    # 3. Cria o atalho mágico 'cmake --build build --target run_...'
    add_custom_target(${run_target_name}
        COMMAND $<TARGET_FILE:${executable_name}>
        DEPENDS ${executable_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Executando Sandbox do modulo: ${run_target_name}"
    )
    
    # 4. Registra no CTest (se quiserem rodar via comando ctest geral)
    add_test(NAME ${run_target_name} COMMAND $<TARGET_FILE:${executable_name}>)
endfunction()

# --- COMPILAÇÃO DOS MÓDULOS (CÓDIGO FONTE) ---

brainrot_add_library(brainrot_parser
    src/parser/FileLoader.cpp
    src/parser/CorpusManager.cpp
)

brainrot_add_library(brainrot_nlp
    src/nlp/SpacyProcessor.cpp
)

brainrot_add_library(brainrot_graph
    src/graph/Graph.cpp
    src/graph/GraphBuilder.cpp
)

brainrot_add_library(brainrot_structures
    src/structures/MaxHeap.cpp
    src/structures/Queue.cpp
)

brainrot_add_library(brainrot_algorithms
    src/algorithms/BFS.cpp
    src/algorithms/Dijkstra.cpp
)

brainrot_add_library(brainrot_analysis
    src/analysis/ReportGenerator.cpp
)

brainrot_add_library(brainrot_utils
    src/utils/Logger.cpp
    src/utils/Config.cpp
)

# --- PROJETO PRINCIPAL INTEGRADO ---

add_executable(BrainrotAnalyzer main.cpp)
target_link_libraries(BrainrotAnalyzer PRIVATE
    brainrot_parser
    brainrot_nlp
    brainrot_graph
    brainrot_structures
    brainrot_algorithms
    brainrot_analysis
    brainrot_utils
)

add_custom_target(run_project
    COMMAND $<TARGET_FILE:BrainrotAnalyzer>
    DEPENDS BrainrotAnalyzer
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running full project"
)

# --- CONEXÃO DOS ATALHOS DE EXECUÇÃO ÀS SANDBOXES DE TESTS/ ---

brainrot_add_runner(run_parser     brainrot_parser     tests/parser/CorpusManagerTest.cpp)
brainrot_add_runner(run_nlp        brainrot_nlp        tests/nlp/SpacyProcessorTest.cpp)
brainrot_add_runner(run_graph      brainrot_graph      tests/graph/GraphTest.cpp)
brainrot_add_runner(run_graph_builder brainrot_graph tests/graph/GraphBuilderTest.cpp)#added
brainrot_add_runner(run_algorithms brainrot_algorithms tests/algorithms/BFSTest.cpp)
brainrot_add_runner(run_structures brainrot_structures tests/structures/MaxHeapTest.cpp)
brainrot_add_runner(run_analysis   brainrot_analysis   tests/analysis/ReportGeneratorTest.cpp)

# Atalho para rodar todas as sandboxes e o projeto um atrás do outro
add_custom_target(run_all
    DEPENDS run_project run_parser run_nlp run_graph run_algorithms run_structures run_analysis
)
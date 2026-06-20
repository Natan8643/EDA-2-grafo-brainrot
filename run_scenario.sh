#!/bin/bash
clear
echo "============================================="
echo "  SELETOR DE CENÁRIOS - BRAINROT ANALYZER   "
echo "============================================="
echo "Escolha um cenário para a apresentação:"
echo "1) Dijkstra (Caminho alternativo de menor custo)"
echo "2) BFS (Níveis de conexões a múltiplos saltos)"
echo "3) MaxHeap (Ranking de relevância de gírias)"
echo "4) Corpus Completo (Análise geral com todas as frases)"
echo "============================================="
read -p "Opção (1-4): " opcao

# Limpa a pasta do corpus
rm -f data/corpus/*

case $opcao in
  1)
    cp data/scenarios/dijkstra.txt data/corpus/doc1.txt
    origem="sigma"
    destino="rizz"
    echo -e "\n[Cenário 1 Selecionado] Dijkstra: sigma -> rizz"
    ;;
  2)
    cp data/scenarios/bfs.txt data/corpus/doc1.txt
    origem="sigma"
    destino="gyatt"
    echo -e "\n[Cenário 2 Selecionado] BFS a partir de sigma"
    ;;
  3)
    cp data/scenarios/relevance.txt data/corpus/doc1.txt
    origem="sigma"
    destino="fanum"
    echo -e "\n[Cenário 3 Selecionado] MaxHeap Ranking"
    ;;
  4)
    cp data/scenarios/full_corpus.txt data/corpus/doc1.txt
    origem="sigma"
    destino="rizz"
    echo -e "\n[Cenário 4 Selecionado] Corpus Completo: sigma -> rizz"
    ;;
  *)
    echo "Opção inválida!"
    exit 1
    ;;
esac

# Compila para garantir que tudo esteja atualizado
cmake --build build --target BrainrotAnalyzer > /dev/null

# Executa passando a origem e destino programaticamente
echo -e "$origem\n$destino" | ./build/BrainrotAnalyzer

# Gera e abre o grafo no navegador
python3 data/visualize.py

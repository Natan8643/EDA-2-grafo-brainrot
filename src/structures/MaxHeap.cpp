#include "structures/MaxHeap.hpp"

#include <stdexcept>
#include <utility> // std::swap

// ---------------------------------------------------------------------------
// MaxHeap: heap binario de maximo sobre std::vector<HeapNode>, ordenado por
// .score. Os auxiliares siftDown/popMax sao funcoes LOCAIS deste .cpp (ficam
// no anonymous namespace), entao nao precisam ser declarados no header.
// Para um no no indice i: pai = (i-1)/2, filhos = 2i+1 e 2i+2.
// ---------------------------------------------------------------------------
namespace
{
    // Desce o elemento em 'index' enquanto for menor que algum filho. O(log n).
    void siftDown(std::vector<HeapNode> &h, size_t index)
    {
        size_t size = h.size();
        while (true)
        {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t largest = index;

            if (left < size && h[left].score > h[largest].score)
                largest = left;
            if (right < size && h[right].score > h[largest].score)
                largest = right;

            if (largest == index)
                break;

            std::swap(h[index], h[largest]);
            index = largest;
        }
    }

    // Remove e retorna o maior elemento (raiz), reorganizando a heap. O(log n).
    HeapNode popMax(std::vector<HeapNode> &h)
    {
        HeapNode top = h.front();
        h.front() = h.back();
        h.pop_back();
        if (!h.empty())
            siftDown(h, 0);
        return top;
    }
}

// Monta a heap a partir do grafo. Score do vertice = soma dos pesos das arestas.
// Heapify bottom-up em O(n).
void MaxHeap::buildFromGraph(const Graph &graph)
{
    heap.clear();

    for (const auto &vertex : graph.getVertices())
    {
        int score = 0;
        for (const auto &edge : graph.getNeighbors(vertex))
            score += edge.weight;
        heap.push_back(HeapNode{vertex, score});
    }

    // Comeca do ultimo no com filho e sobe ate a raiz (o "--i" evita underflow).
    for (size_t i = heap.size() / 2; i > 0;)
    {
        --i;
        siftDown(heap, i);
    }
}

// Remove e retorna o no de maior score. Lanca excecao se a heap estiver vazia.
HeapNode MaxHeap::extractMax()
{
    if (heap.empty())
        throw std::out_of_range("MaxHeap::extractMax chamado em uma heap vazia");
    return popMax(heap);
}

// Retorna os k maiores scores, em ordem decrescente, SEM destruir a heap.
// Trabalha sobre uma copia local. O(k log n).
std::vector<HeapNode> MaxHeap::getTopK(int k) const
{
    std::vector<HeapNode> result;
    if (k <= 0 || heap.empty())
        return result;

    std::vector<HeapNode> copy = heap; // copia para preservar a heap original

    size_t count = (static_cast<size_t>(k) < copy.size())
                       ? static_cast<size_t>(k)
                       : copy.size();

    result.reserve(count);
    for (size_t i = 0; i < count; ++i)
        result.push_back(popMax(copy));

    return result;
}

bool MaxHeap::empty() const
{
    return heap.empty();
}
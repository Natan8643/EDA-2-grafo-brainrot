#include "structures/Queue.hpp"

#include "structures/Queue.hpp"

#include <stdexcept>
#include <string>
#include <utility>

// ---------------------------------------------------------------------------
// Implementacao dos metodos do template Queue<T>.
// Como o projeto mantem a implementacao em src/ (e nao no header), usamos
// INSTANCIACAO EXPLICITA no final do arquivo: uma linha por tipo de Queue<T>
// realmente usado no projeto. Assim o compilador gera os simbolos e o linker
// encontra tudo, sem precisar do corpo no header.
//
// Estrategia da fila: 'frontIndex' marca o inicio logico, evitando remover do
// comeco do vetor (O(n)). Ao esvaziar, o vetor e limpo e o indice zerado.
// Todas as operacoes sao O(1) amortizado.
// ---------------------------------------------------------------------------

template <typename T>
Queue<T>::Queue() : frontIndex(0) {}

template <typename T>
void Queue<T>::enqueue(const T &value)
{
    data.push_back(value);
}

template <typename T>
void Queue<T>::dequeue()
{
    if (empty())
        throw std::out_of_range("Queue::dequeue chamado em uma fila vazia");

    ++frontIndex;

    // Quando a fila esvazia, reaproveita o vetor para nao crescer sem limite.
    if (frontIndex == data.size())
    {
        data.clear();
        frontIndex = 0;
    }
}

template <typename T>
T &Queue<T>::front()
{
    if (empty())
        throw std::out_of_range("Queue::front chamado em uma fila vazia");

    return data[frontIndex];
}

template <typename T>
bool Queue<T>::empty() const
{
    return frontIndex >= data.size();
}

template <typename T>
size_t Queue<T>::size() const
{
    return data.size() - frontIndex;
}

// --- Instanciacao explicita: uma linha por tipo de Queue<T> usado no projeto ---
template class Queue<std::string>;                  // uso geral / testes
template class Queue<std::pair<std::string, int>>;  // usado pelo BFS (vertice, nivel)
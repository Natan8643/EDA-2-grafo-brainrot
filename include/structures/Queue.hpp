/*
Arquivo de Cabeçalho: define o padrão/contrato a ser seguido
Deve ser implementado no seu respectivo arquivo na pasta src

interface da fila que sera utilizado no bfs



*/

#pragma once

#include <vector>

template <typename T>
class Queue
{
private:
    std::vector<T> data;
    size_t frontIndex;

public:
    Queue();

    void enqueue(const T &value);

    void dequeue();

    T &front();

    bool empty() const;

    size_t size() const;
};
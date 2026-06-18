#include <iostream>
#include <cassert>
#include <cmath>
#include "graph/Graph.hpp"
#include "algorithms/Dijkstra.hpp"

static int testsPassed = 0;
static int testsFailed = 0;

#define TEST(name) \
    std::cout << "  " << name << "... "; \
    try {
#define ENDTEST() \
        std::cout << "OK" << std::endl; \
        testsPassed++; \
    } catch (const std::exception &e) { \
        std::cout << "FAIL (" << e.what() << ")" << std::endl; \
        testsFailed++; \
    } catch (...) { \
        std::cout << "FAIL (unknown)" << std::endl; \
        testsFailed++; \
    }

void test_direto()
{
    TEST("aresta direta unica")
    Graph g;
    g.addEdge("sigma", "ohio", 5);
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    assert(r.totalWeight == 5);
    assert(r.path.size() == 2);
    assert(r.path[0] == "sigma");
    assert(r.path[1] == "ohio");
    ENDTEST()
}

void test_multiplos_caminhos()
{
    TEST("multiplos caminhos — escolhe o menor")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    g.addEdge("sigma", "rizz", 5);
    g.addEdge("sigma", "ohio", 10);
    g.addEdge("aura", "rizz", 1);
    g.addEdge("rizz", "ohio", 3);
    g.addEdge("rizz", "grindset", 1);

    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    assert(r.totalWeight == 5);
    assert(r.path.front() == "sigma");
    assert(r.path.back() == "ohio");
    // sigma -> aura -> rizz -> ohio
    ENDTEST()
}

void test_sem_caminho()
{
    TEST("sem caminho entre origem e destino")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    g.addEdge("rizz", "ohio", 2);
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(!r.found);
    assert(r.path.empty());
    ENDTEST()
}

void test_vertice_inexistente()
{
    TEST("vertice de origem nao existe no grafo")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    auto r = Dijkstra().findShortestPath(g, "inventado", "sigma");
    assert(!r.found);
    assert(r.path.empty());
    ENDTEST()
}

void test_destino_inexistente()
{
    TEST("vertice de destino nao existe no grafo")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    auto r = Dijkstra().findShortestPath(g, "sigma", "inventado");
    assert(!r.found);
    assert(r.path.empty());
    ENDTEST()
}

void test_origem_igual_destino()
{
    TEST("origem igual ao destino")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    auto r = Dijkstra().findShortestPath(g, "sigma", "sigma");
    assert(r.found);
    assert(r.totalWeight == 0);
    assert(r.path.size() == 1);
    assert(r.path[0] == "sigma");
    ENDTEST()
}

void test_vertice_unico()
{
    TEST("unico vertice no grafo, sem arestas, origem == destino")
    Graph g;
    g.addVertex("sigma");
    auto r = Dijkstra().findShortestPath(g, "sigma", "sigma");
    assert(r.found);
    assert(r.totalWeight == 0);
    assert(r.path.size() == 1);
    ENDTEST()
}

void test_grafo_vazio()
{
    TEST("grafo vazio — origem nao existe")
    Graph g;
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(!r.found);
    assert(r.path.empty());
    ENDTEST()
}

void test_caminho_unico_com_varios_pesos()
{
    TEST("cadeia longa unica")
    Graph g;
    g.addEdge("a", "b", 3);
    g.addEdge("b", "c", 7);
    g.addEdge("c", "d", 2);
    g.addEdge("d", "e", 1);
    auto r = Dijkstra().findShortestPath(g, "a", "e");
    assert(r.found);
    assert(r.totalWeight == 13);
    assert(r.path.size() == 5);
    assert(r.path[0] == "a");
    assert(r.path[4] == "e");
    ENDTEST()
}

void test_caminhos_multiplos_empatados()
{
    TEST("dois caminhos com o mesmo peso")
    Graph g;
    g.addEdge("sigma", "aura", 5);
    g.addEdge("sigma", "rizz", 5);
    g.addEdge("aura", "ohio", 3);
    g.addEdge("rizz", "ohio", 3);
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    assert(r.totalWeight == 8);
    assert(r.path.front() == "sigma");
    assert(r.path.back() == "ohio");
    ENDTEST()
}

void test_grafo_grande()
{
    TEST("grafo maior com multiplas rotas")
    //        a --4-- b --2-- c
    //       / \               |
    //      1   2              6
    //     /     \             |
    // sigma--5-- d --3-- e ---+
    //     \           /
    //      3         4
    //       \       /
    //        f --1-- g --2-- ohio
    Graph g;
    g.addEdge("sigma", "a", 1);
    g.addEdge("sigma", "d", 5);
    g.addEdge("sigma", "f", 3);
    g.addEdge("a", "b", 4);
    g.addEdge("a", "d", 2);
    g.addEdge("b", "c", 2);
    g.addEdge("c", "ohio", 6);
    g.addEdge("d", "e", 3);
    g.addEdge("e", "c", 6);
    g.addEdge("e", "ohio", 6);
    g.addEdge("f", "g", 1);
    g.addEdge("g", "ohio", 2);
    g.addEdge("f", "e", 4);

    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    // sigma -> f -> g -> ohio = 3 + 1 + 2 = 6  (* menor)
    assert(r.totalWeight == 6);
    assert(r.path.front() == "sigma");
    assert(r.path.back() == "ohio");
    ENDTEST()
}

void test_caminho_indireto_mais_leve()
{
    TEST("caminho indireto mais leve que o direto")
    Graph g;
    g.addEdge("sigma", "ohio", 100);
    g.addEdge("sigma", "aura", 1);
    g.addEdge("aura", "rizz", 1);
    g.addEdge("rizz", "ohio", 1);
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    assert(r.totalWeight == 3);
    assert(r.path.size() == 4);
    ENDTEST()
}

void test_pesos_decimais()
{
    TEST("pesos inteiros (Edge usa int, entao pesos decimais truncam)")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    g.addEdge("aura", "ohio", 2);
    g.addEdge("sigma", "ohio", 5);
    auto r = Dijkstra().findShortestPath(g, "sigma", "ohio");
    assert(r.found);
    assert(r.totalWeight == 3);
    assert(r.path.size() == 3);
    ENDTEST()
}

int main()
{
    std::cout << "Testes Dijkstra:" << std::endl;

    test_direto();
    test_multiplos_caminhos();
    test_sem_caminho();
    test_vertice_inexistente();
    test_destino_inexistente();
    test_origem_igual_destino();
    test_vertice_unico();
    test_grafo_vazio();
    test_caminho_unico_com_varios_pesos();
    test_caminhos_multiplos_empatados();
    test_grafo_grande();
    test_caminho_indireto_mais_leve();
    test_pesos_decimais();

    std::cout << "\nResultado: " << (testsPassed + testsFailed) << " testes, "
              << testsPassed << " passaram, " << testsFailed << " falharam." << std::endl;
    return testsFailed > 0 ? 1 : 0;
}

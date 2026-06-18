#include <iostream>
#include <cassert>
#include "graph/Graph.hpp"
#include "algorithms/BFS.hpp"

static int testsPassed = 0;
static int testsFailed = 0;

#define TEST(name) \
    std::cout << "  " << name << "... "; \
    try {
#define ENDTest() \
        std::cout << "OK" << std::endl; \
        testsPassed++; \
    } catch (const std::exception &e) { \
        std::cout << "FAIL (" << e.what() << ")" << std::endl; \
        testsFailed++; \
    } catch (...) { \
        std::cout << "FAIL (unknown error)" << std::endl; \
        testsFailed++; \
    }

void test_simple_chain()
{
    TEST("cadeia simples sigma -> aura -> ohio")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    g.addEdge("aura", "ohio", 1);

    BFS bfs;
    auto r = bfs.traverse(g, "sigma");

    assert(r.size() == 3);
    assert(r[0].word == "sigma" && r[0].level == 0);
    assert(r[1].word == "aura" && r[1].level == 1);
    assert(r[2].word == "ohio" && r[2].level == 2);
    ENDTest()
}

void test_vertex_inexistente()
{
    TEST("startVertex nao existe no grafo")
    Graph g;
    g.addVertex("sigma");
    BFS bfs;
    auto r = bfs.traverse(g, "aura");
    assert(r.empty());
    ENDTest()
}

void test_vertice_unico()
{
    TEST("grafo com um unico vertice")
    Graph g;
    g.addVertex("sigma");
    BFS bfs;
    auto r = bfs.traverse(g, "sigma");
    assert(r.size() == 1);
    assert(r[0].word == "sigma" && r[0].level == 0);
    ENDTest()
}

void test_ciclo()
{
    TEST("grafo com ciclo")
    Graph g;
    g.addEdge("a", "b", 1);
    g.addEdge("b", "c", 1);
    g.addEdge("c", "a", 1);
    BFS bfs;
    auto r = bfs.traverse(g, "a");
    assert(r.size() == 3);
    for (const auto &x : r)
        std::cout << "      " << x.word << " nivel " << x.level << std::endl;
    ENDTest()
}

void test_grafo_desconexo()
{
    TEST("grafo desconexo — BFS so visita um componente")
    Graph g;
    g.addEdge("sigma", "aura", 1);
    g.addEdge("rizz", "ohio", 1);
    BFS bfs;
    auto r = bfs.traverse(g, "sigma");
    assert(r.size() == 2);
    assert(r[0].word == "sigma");
    assert(r[1].word == "aura");
    ENDTest()
}

void test_multiplas_chamadas()
{
    TEST("chamar traverse duas vezes no mesmo objeto")
    Graph g;
    g.addEdge("a", "b", 1);
    g.addEdge("b", "c", 1);
    BFS bfs;
    auto r1 = bfs.traverse(g, "a");
    auto r2 = bfs.traverse(g, "c");
    assert(r1.size() == 3 && r2.size() == 3);
    assert(r2[0].word == "c" && r2[0].level == 0);
    ENDTest()
}

void test_grafo_vazio()
{
    TEST("grafo vazio (nenhum vertice)")
    Graph g;
    BFS bfs;
    auto r = bfs.traverse(g, "sigma");
    assert(r.empty());
    ENDTest()
}

int main()
{
    std::cout << "Testes BFS:" << std::endl;
    test_simple_chain();
    test_vertex_inexistente();
    test_vertice_unico();
    test_ciclo();
    test_grafo_desconexo();
    test_multiplas_chamadas();
    test_grafo_vazio();

    std::cout << "\nResultado: " << (testsPassed + testsFailed) << " testes, "
              << testsPassed << " passaram, " << testsFailed << " falharam." << std::endl;
    return testsFailed > 0 ? 1 : 0;
}

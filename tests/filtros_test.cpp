#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/filtros.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Filtra tarefas por status", "[Filtros]") {
    Tarefa t1("Tarefa 1");
    Tarefa t2("Tarefa 2");
    t2.concluir();

    std::vector<Tarefa> tarefas = {t1, t2};
    auto pendentes = Filtros::porStatus(tarefas, Status::Pendente);
    auto concluidas = Filtros::porStatus(tarefas, Status::Concluida);

    REQUIRE(pendentes.size() == 1);
    REQUIRE(concluidas.size() == 1);
    REQUIRE(pendentes[0].titulo() == "Tarefa 1");
    REQUIRE(concluidas[0].titulo() == "Tarefa 2");
}

TEST_CASE("Filtra tarefas por prioridade", "[Filtros]") {
    Tarefa t1("Tarefa 1", std::nullopt, Prioridade::Alta);
    Tarefa t2("Tarefa 2", std::nullopt, Prioridade::Baixa);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto altas = Filtros::porPrioridade(tarefas, Prioridade::Alta);

    REQUIRE(altas.size() == 1);
    REQUIRE(altas[0].titulo() == "Tarefa 1");
}

TEST_CASE("Filtra tarefas por etiqueta", "[Filtros]") {
    Tarefa t1("Tarefa 1");
    t1.adicionarEtiqueta("trabalho");

    Tarefa t2("Tarefa 2");
    t2.adicionarEtiqueta("pessoal");

    std::vector<Tarefa> tarefas = {t1, t2};
    auto trabalho = Filtros::porEtiqueta(tarefas, "trabalho");

    REQUIRE(trabalho.size() == 1);
    REQUIRE(trabalho[0].titulo() == "Tarefa 1");
}
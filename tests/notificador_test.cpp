#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/notificador.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Notificações para tarefas atrasadas", "[Notificador]") {
    Tarefa t1("Tarefa atrasada");
    auto agora = std::chrono::system_clock::now();
    auto ontem = agora - std::chrono::hours(24);
    t1.definirDataVencimento(ontem);

    Tarefa t2("Tarefa dentro do prazo");
    auto amanha = agora + std::chrono::hours(24);
    t2.definirDataVencimento(amanha);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto notificacoes = Notificador::notificacoesAtrasadas(tarefas);

    REQUIRE(notificacoes.size() == 1);
    REQUIRE(notificacoes[0] == "A tarefa \"Tarefa atrasada\" está atrasada!");
}

TEST_CASE("Notificações para tarefas próximas de vencer", "[Notificador]") {
    Tarefa t1("Tarefa próxima de vencer");
    auto agora = std::chrono::system_clock::now();
    auto amanha = agora + std::chrono::hours(24);
    t1.definirDataVencimento(amanha);

    Tarefa t2("Tarefa fora do prazo");
    auto daquiTresDias = agora + std::chrono::hours(24 * 3);
    t2.definirDataVencimento(daquiTresDias);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto notificacoes = Notificador::notificacoesProximasDeVencer(tarefas, 2);

    REQUIRE(notificacoes.size() == 1);
    REQUIRE(notificacoes[0] == "A tarefa \"Tarefa próxima de vencer\" está próxima de vencer!");
}
#include "afazer/dominio/tarefa.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Tarefa é criada com título válido", "[Tarefa]") {
    Tarefa tarefa("Estudar C++");
    REQUIRE(tarefa.titulo() == "Estudar C++");
    REQUIRE(tarefa.status() == Status::Pendente);
}

TEST_CASE("Tarefa não pode ser criada com título vazio", "[Tarefa]") {
    REQUIRE_THROWS_AS(Tarefa(""), std::invalid_argument);
}

TEST_CASE("Tarefa pode ser concluída", "[Tarefa]") {
    Tarefa tarefa("Estudar C++");
    tarefa.concluir();
    REQUIRE(tarefa.status() == Status::Concluida);
}

TEST_CASE("Tarefa pode ser cancelada", "[Tarefa]") {
    Tarefa tarefa("Estudar C++");
    tarefa.cancelar();
    REQUIRE(tarefa.status() == Status::Cancelada);
}

TEST_CASE("Tarefa com periodicidade diária calcula próxima ocorrência", "[Tarefa]") {
    using namespace afazer::dominio;

    Tarefa tarefa("Tarefa diária");
    tarefa.definirPeriodicidade(Periodicidade::Diaria);
    tarefa.concluir();

    REQUIRE(tarefa.proximaOcorrencia().has_value());
    REQUIRE(tarefa.periodicidade() == Periodicidade::Diaria);
}

TEST_CASE("Tarefa sem periodicidade não calcula próxima ocorrência", "[Tarefa]") {
    using namespace afazer::dominio;

    Tarefa tarefa("Tarefa única");
    tarefa.definirPeriodicidade(Periodicidade::Nenhuma);
    tarefa.concluir();

    REQUIRE_FALSE(tarefa.proximaOcorrencia().has_value());
}

TEST_CASE("Tarefa com data de vencimento atrasada", "[Tarefa]") {
    using namespace afazer::dominio;

    Tarefa tarefa("Tarefa atrasada");
    auto agora = std::chrono::system_clock::now();
    auto ontem = agora - std::chrono::hours(24);
    tarefa.definirDataVencimento(ontem);

    REQUIRE(tarefa.estaAtrasada());
}

TEST_CASE("Tarefa com data de vencimento próxima", "[Tarefa]") {
    using namespace afazer::dominio;

    Tarefa tarefa("Tarefa próxima de vencer");
    auto agora = std::chrono::system_clock::now();
    auto amanha = agora + std::chrono::hours(24);
    tarefa.definirDataVencimento(amanha);

    REQUIRE(tarefa.estaProximaDeVencer(1));
    REQUIRE_FALSE(tarefa.estaAtrasada());
}

TEST_CASE("Tarefa sem data de vencimento não está atrasada", "[Tarefa]") {
    using namespace afazer::dominio;

    Tarefa tarefa("Tarefa sem vencimento");
    REQUIRE_FALSE(tarefa.estaAtrasada());
}
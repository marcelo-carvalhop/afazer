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
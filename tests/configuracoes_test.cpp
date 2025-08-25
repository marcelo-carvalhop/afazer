#include "afazer/dominio/configuracoes.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Definir e obter configurações", "[Configuracoes]") {
    Configuracoes config;
    config.definir("dias_notificacao", "5");
    config.definir("formato_exportacao", "csv");

    REQUIRE(config.obter("dias_notificacao") == "5");
    REQUIRE(config.obter("formato_exportacao") == "csv");
}

TEST_CASE("Obter configuração com valor padrão", "[Configuracoes]") {
    Configuracoes config;

    REQUIRE(config.obterComPadrao("dias_notificacao", "3") == "3");
    config.definir("dias_notificacao", "7");
    REQUIRE(config.obterComPadrao("dias_notificacao", "3") == "7");
}

TEST_CASE("Erro ao obter configuração inexistente", "[Configuracoes]") {
    Configuracoes config;

    REQUIRE_THROWS_AS(config.obter("inexistente"), std::runtime_error);
}
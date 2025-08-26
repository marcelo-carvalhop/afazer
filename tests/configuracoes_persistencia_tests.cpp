#include "afazer/dominio/configuracoes.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>

using namespace afazer::dominio;

TEST_CASE("Salvar e carregar configurações de um arquivo", "[Configuracoes]") {
    Configuracoes config;

    // Define algumas configurações
    config.definir("dias_notificacao", "5");
    config.definir("formato_exportacao", "csv");

    // Salva as configurações em um arquivo temporário
    const std::string caminhoArquivo = "configuracoes_teste.json";
    config.salvarEmArquivo(caminhoArquivo);

    // Carrega as configurações de um novo objeto
    Configuracoes configCarregado;
    configCarregado.carregarDeArquivo(caminhoArquivo);

    // Verifica se as configurações foram carregadas corretamente
    REQUIRE(configCarregado.obter("dias_notificacao") == "5");
    REQUIRE(configCarregado.obter("formato_exportacao") == "csv");

    // Remove o arquivo temporário
    std::remove(caminhoArquivo.c_str());
}

TEST_CASE("Erro ao carregar configurações de um arquivo inexistente", "[Configuracoes]") {
    Configuracoes config;

    REQUIRE_THROWS_AS(config.carregarDeArquivo("arquivo_inexistente.json"), std::runtime_error);
}
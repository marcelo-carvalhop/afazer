#include "afazer/dominio/logger.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>

using namespace afazer::dominio;

TEST_CASE("Exportar logs em JSON", "[Logger]") {
    const std::string caminhoArquivo = "teste_log.json";

    Logger logger(caminhoArquivo);
    logger.registrarComNivel("INFO", "Mensagem de teste 1");
    logger.registrarComNivel("WARNING", "Mensagem de teste 2");

    auto jsonLogs = logger.exportarJSON();
    REQUIRE(jsonLogs.find("Mensagem de teste 1") != std::string::npos);
    REQUIRE(jsonLogs.find("Mensagem de teste 2") != std::string::npos);

    std::remove(caminhoArquivo.c_str());
}

TEST_CASE("Exportar logs em CSV", "[Logger]") {
    const std::string caminhoArquivo = "teste_log.csv";

    Logger logger(caminhoArquivo);
    logger.registrarComNivel("INFO", "Mensagem de teste 1");
    logger.registrarComNivel("WARNING", "Mensagem de teste 2");

    auto csvLogs = logger.exportarCSV();
    REQUIRE(csvLogs.find("Mensagem de teste 1") != std::string::npos);
    REQUIRE(csvLogs.find("Mensagem de teste 2") != std::string::npos);

    std::remove(caminhoArquivo.c_str());
}
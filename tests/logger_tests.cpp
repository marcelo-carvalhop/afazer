#include "afazer/dominio/logger.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <string>

using namespace afazer::dominio;

TEST_CASE("Registrar mensagens no log", "[Logger]") {
    const std::string caminhoArquivo = "teste_log.log";

    // Cria o logger
    Logger logger(caminhoArquivo);

    // Registra mensagens
    logger.registrar("Mensagem de teste 1");
    logger.registrarComNivel("INFO", "Mensagem de teste 2");

    // Verifica se as mensagens foram registradas
    std::ifstream arquivo(caminhoArquivo);
    REQUIRE(arquivo.is_open());

    std::string linha;
    std::getline(arquivo, linha);
    REQUIRE(linha.find("Mensagem de teste 1") != std::string::npos);

    std::getline(arquivo, linha);
    REQUIRE(linha.find("[INFO] Mensagem de teste 2") != std::string::npos);

    // Remove o arquivo de log
    std::remove(caminhoArquivo.c_str());
}
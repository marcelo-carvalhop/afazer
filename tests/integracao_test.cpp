#include "afazer/dominio/configuracoes.hpp"
#include "afazer/dominio/notificador.hpp"
#include "afazer/dominio/exportador.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Notificador usa configuração de dias para notificações", "[Integracao]") {
    Configuracoes config;
    config.definir("dias_notificacao", "5");

    Notificador notificador(config);

    Tarefa t1("Tarefa próxima de vencer");
    auto agora = std::chrono::system_clock::now();
    auto daquiCincoDias = agora + std::chrono::hours(24 * 5);
    t1.definirDataVencimento(daquiCincoDias);

    std::vector<Tarefa> tarefas = {t1};
    auto notificacoes = notificador.notificacoesProximasDeVencer(tarefas);

    REQUIRE(notificacoes.size() == 1);
    REQUIRE(notificacoes[0] == "A tarefa \"Tarefa próxima de vencer\" está próxima de vencer!");
}

TEST_CASE("Exportador usa configuração de formato", "[Integracao]") {
    Configuracoes config;
    config.definir("formato_exportacao", "csv");

    Exportador exportador(config);

    Tarefa t1("Tarefa 1");
    std::vector<Tarefa> tarefas = {t1};

    auto exportado = exportador.exportar(tarefas);

    REQUIRE(exportado.find("Tarefa 1") != std::string::npos);
    REQUIRE(exportado.find("ID,Título,Descrição") != std::string::npos); // Verifica se é CSV
}
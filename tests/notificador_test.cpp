#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/notificador.hpp"
#include "afazer/dominio/configuracoes.hpp"
#include <catch2/catch_test_macros.hpp>
#include "afazer/dominio/logger.hpp"

using namespace afazer::dominio;

TEST_CASE("Notificações para tarefas próximas de vencer", "[Notificador]") {
    Configuracoes config;
    config.definir("dias_notificacao", "2");

    Notificador notificador(config);

    // Certifique-se de que o caminho do arquivo de log é válido
    Logger logger("logs/teste_log.json");

    Tarefa t1("Tarefa próxima de vencer");
    auto agora = std::chrono::system_clock::now();
    auto daquiDoisDias = agora + std::chrono::hours(24 * 2);
    t1.definirDataVencimento(daquiDoisDias);

    std::vector<Tarefa> tarefas = {t1};
    auto notificacoes = notificador.notificacoesProximasDeVencer(tarefas);

    REQUIRE(notificacoes.size() == 1);
    REQUIRE(notificacoes[0] == "A tarefa \"Tarefa próxima de vencer\" está próxima de vencer!");
}
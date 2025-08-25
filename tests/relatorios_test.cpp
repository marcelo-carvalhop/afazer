#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/relatorios.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Relatório de tarefas por status", "[Relatorios]") {
    Tarefa t1("Tarefa 1");
    Tarefa t2("Tarefa 2");
    t2.concluir();

    std::vector<Tarefa> tarefas = {t1, t2};
    auto pendentes = Relatorios::tarefasPorStatus(tarefas, Status::Pendente);
    auto concluidas = Relatorios::tarefasPorStatus(tarefas, Status::Concluida);

    REQUIRE(pendentes.size() == 1);
    REQUIRE(concluidas.size() == 1);
    REQUIRE(pendentes[0].titulo() == "Tarefa 1");
    REQUIRE(concluidas[0].titulo() == "Tarefa 2");
}

TEST_CASE("Relatório de tarefas por prioridade", "[Relatorios]") {
    Tarefa t1("Tarefa 1", std::nullopt, Prioridade::Alta);
    Tarefa t2("Tarefa 2", std::nullopt, Prioridade::Baixa);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto altas = Relatorios::tarefasPorPrioridade(tarefas, Prioridade::Alta);

    REQUIRE(altas.size() == 1);
    REQUIRE(altas[0].titulo() == "Tarefa 1");
}

TEST_CASE("Contagem de tarefas por status", "[Relatorios]") {
    Tarefa t1("Tarefa 1");
    Tarefa t2("Tarefa 2");
    t2.concluir();

    std::vector<Tarefa> tarefas = {t1, t2};
    auto contagem = Relatorios::contagemPorStatus(tarefas);

    REQUIRE(contagem[Status::Pendente] == 1);
    REQUIRE(contagem[Status::Concluida] == 1);
}

TEST_CASE("Relatório de tarefas por etiqueta", "[Relatorios]") {
    Tarefa t1("Tarefa 1");
    t1.adicionarEtiqueta("trabalho");

    Tarefa t2("Tarefa 2");
    t2.adicionarEtiqueta("pessoal");

    std::vector<Tarefa> tarefas = {t1, t2};
    auto agrupadas = Relatorios::tarefasPorEtiqueta(tarefas);

    REQUIRE(agrupadas["trabalho"].size() == 1);
    REQUIRE(agrupadas["pessoal"].size() == 1);
    REQUIRE(agrupadas["trabalho"][0].titulo() == "Tarefa 1");
    REQUIRE(agrupadas["pessoal"][0].titulo() == "Tarefa 2");
}

TEST_CASE("Exporta contagem por status para JSON", "[Relatorios]") {
    std::map<Status, int> contagem = {
        {Status::Pendente, 3},
        {Status::Concluida, 2},
        {Status::Cancelada, 1}
    };

    auto json = Relatorios::exportarContagemPorStatusJSON(contagem);

    REQUIRE(json.find("\"0\": 3") != std::string::npos); // Status::Pendente
    REQUIRE(json.find("\"1\": 2") != std::string::npos); // Status::Concluida
    REQUIRE(json.find("\"2\": 1") != std::string::npos); // Status::Cancelada
}

TEST_CASE("Exporta tarefas por etiqueta para JSON", "[Relatorios]") {
    Tarefa t1("Tarefa 1");
    t1.adicionarEtiqueta("trabalho");

    Tarefa t2("Tarefa 2");
    t2.adicionarEtiqueta("pessoal");

    std::map<std::string, std::vector<Tarefa>> agrupadas = {
        {"trabalho", {t1}},
        {"pessoal", {t2}}
    };

    auto json = Relatorios::exportarTarefasPorEtiquetaJSON(agrupadas);

    REQUIRE(json.find("Tarefa 1") != std::string::npos);
    REQUIRE(json.find("Tarefa 2") != std::string::npos);
    REQUIRE(json.find("trabalho") != std::string::npos);
    REQUIRE(json.find("pessoal") != std::string::npos);
}

TEST_CASE("Exporta tarefas por etiqueta para CSV", "[Relatorios]") {
    Tarefa t1("Tarefa 1");
    t1.adicionarEtiqueta("trabalho");

    Tarefa t2("Tarefa 2");
    t2.adicionarEtiqueta("pessoal");

    std::map<std::string, std::vector<Tarefa>> agrupadas = {
        {"trabalho", {t1}},
        {"pessoal", {t2}}
    };

    auto csv = Relatorios::exportarTarefasPorEtiquetaCSV(agrupadas);

    REQUIRE(csv.find("Tarefa 1") != std::string::npos);
    REQUIRE(csv.find("Tarefa 2") != std::string::npos);
    REQUIRE(csv.find("trabalho") != std::string::npos);
    REQUIRE(csv.find("pessoal") != std::string::npos);
}

TEST_CASE("Relatório de tarefas atrasadas", "[Relatorios]") {
    using namespace afazer::dominio;

    Tarefa t1("Tarefa atrasada");
    auto agora = std::chrono::system_clock::now();
    auto ontem = agora - std::chrono::hours(24);
    t1.definirDataVencimento(ontem);

    Tarefa t2("Tarefa dentro do prazo");
    auto amanha = agora + std::chrono::hours(24);
    t2.definirDataVencimento(amanha);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto atrasadas = Relatorios::tarefasAtrasadas(tarefas);

    REQUIRE(atrasadas.size() == 1);
    REQUIRE(atrasadas[0].titulo() == "Tarefa atrasada");
}

TEST_CASE("Relatório de tarefas próximas de vencer", "[Relatorios]") {
    using namespace afazer::dominio;

    Tarefa t1("Tarefa próxima de vencer");
    auto agora = std::chrono::system_clock::now();
    auto amanha = agora + std::chrono::hours(24);
    t1.definirDataVencimento(amanha);

    Tarefa t2("Tarefa fora do prazo");
    auto daquiTresDias = agora + std::chrono::hours(24 * 3);
    t2.definirDataVencimento(daquiTresDias);

    std::vector<Tarefa> tarefas = {t1, t2};
    auto proximas = Relatorios::tarefasProximasDeVencer(tarefas, 2);

    REQUIRE(proximas.size() == 1);
    REQUIRE(proximas[0].titulo() == "Tarefa próxima de vencer");
}
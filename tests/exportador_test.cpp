#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/exportador.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace afazer::dominio;

TEST_CASE("Exporta tarefas para JSON", "[Exportador]") {
    Tarefa t1("Tarefa 1", "Descrição 1", Prioridade::Alta);
    Tarefa t2("Tarefa 2", "Descrição 2", Prioridade::Baixa);
    t1.adicionarEtiqueta("trabalho");
    t2.adicionarEtiqueta("pessoal");

    std::vector<Tarefa> tarefas = {t1, t2};
    auto json = Exportador::paraJSON(tarefas);

    REQUIRE(json.find("Tarefa 1") != std::string::npos);
    REQUIRE(json.find("Descrição 1") != std::string::npos);
    REQUIRE(json.find("trabalho") != std::string::npos);
}

TEST_CASE("Exporta tarefas para CSV", "[Exportador]") {
    Tarefa t1("Tarefa 1", "Descrição 1", Prioridade::Alta);
    Tarefa t2("Tarefa 2", "Descrição 2", Prioridade::Baixa);
    t1.adicionarEtiqueta("trabalho");
    t2.adicionarEtiqueta("pessoal");

    std::vector<Tarefa> tarefas = {t1, t2};
    auto csv = Exportador::paraCSV(tarefas);

    REQUIRE(csv.find("Tarefa 1") != std::string::npos);
    REQUIRE(csv.find("Descrição 1") != std::string::npos);
    REQUIRE(csv.find("trabalho") != std::string::npos);
}

TEST_CASE("Importa tarefas de JSON", "[Exportador]") {
    std::string jsonString = R"([
        {
            "id": 1,
            "titulo": "Tarefa 1",
            "descricao": "Descrição 1",
            "status": 0,
            "prioridade": 2,
            "dataCriacao": 1692873600,
            "dataConclusao": null,
            "etiquetas": ["trabalho", "urgente"]
        },
        {
            "id": 2,
            "titulo": "Tarefa 2",
            "descricao": "Descrição 2",
            "status": 1,
            "prioridade": 1,
            "dataCriacao": 1692877200,
            "dataConclusao": 1692880800,
            "etiquetas": ["pessoal"]
        }
    ])";

    auto tarefas = Exportador::deJSON(jsonString);

    REQUIRE(tarefas.size() == 2);
    REQUIRE(tarefas[0].titulo() == "Tarefa 1");
    REQUIRE(tarefas[1].titulo() == "Tarefa 2");
}

TEST_CASE("Importa tarefas de CSV", "[Exportador]") {
    std::string csvString = R"(ID,Título,Descrição,Status,Prioridade,Data de Criação,Data de Conclusão,Etiquetas
1,"Tarefa 1","Descrição 1",0,2,1692873600,null,"trabalho;urgente"
2,"Tarefa 2","Descrição 2",1,1,1692877200,1692880800,"pessoal"
)";

    auto tarefas = Exportador::deCSV(csvString);

    REQUIRE(tarefas.size() == 2);
    REQUIRE(tarefas[0].titulo() == "Tarefa 1");
    REQUIRE(tarefas[1].titulo() == "Tarefa 2");
}
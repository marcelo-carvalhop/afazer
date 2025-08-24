#pragma once
#include "afazer/dominio/tarefa.hpp"
#include <string>
#include <vector>

namespace afazer::dominio {

    class Exportador {
    public:
        // Exporta uma lista de tarefas para JSON
        static std::string paraJSON(const std::vector<Tarefa>& tarefas);

        //Exporta uma lista de tarefas para CSV
        static std::string paraCSV(const std::vector<Tarefa>& tarefas);

        // Importa uma lista de tarefas de um JSON
        static std::vector<Tarefa> deJSON(const std::string& jsonString);

        // Importa uma lista de tarefas de um CSV
        static std::vector<Tarefa> deCSV(const std::string& csvString);
    };

} // Namespace afazer::dominio
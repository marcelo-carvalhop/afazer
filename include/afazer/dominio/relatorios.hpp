#pragma once
#include "afazer/dominio/tarefa.hpp"
#include <string>
#include <vector>
#include <map>

namespace afazer::dominio {

    class Relatorios {
    public:
        // Relatório de tarefas por status
        static std::vector<Tarefa> tarefasPorStatus(const std::vector<Tarefa>& tarefas, Status status);

        // Relatório de tarefas por prioridade
        static std::vector<Tarefa> tarefasPorPrioridade(const std::vector<Tarefa>& tarefas, Prioridade prioridade);

        // Contagem de tarefas por status
        static std::map<Status, int> contagemPorStatus(const std::vector<Tarefa>& tarefas);

        // Contagem de tarefas por prioridade
        static std::map<Prioridade, int> contagemPorPrioridade(const std::vector<Tarefa>& tarefas);

        // Relatório de tarefas agrupadas por etiqueta
        static std::map<std::string, std::vector<Tarefa>> tarefasPorEtiqueta(const std::vector<Tarefa>& tarefas);

        // Relatório de tarefas atrasadas
        static std::vector<Tarefa> tarefasAtrasadas(const std::vector<Tarefa>& tarefas);

        // Relatório de tarefas próximas de vencer
        static std::vector<Tarefa> tarefasProximasDeVencer(const std::vector<Tarefa>& tarefas, int dias);

        // Exporta relatório de contagem por status para JSON
        static std::string exportarContagemPorStatusJSON(const std::map<Status, int>& contagem);

        // Exporta relatório de contagem por prioridade para JSON
        static std::string exportarContagemPorPrioridadeJSON(const std::map<Prioridade, int>& contagem);

        // Exporta relatório de tarefas agrupadas por etiqueta para JSON
        static std::string exportarTarefasPorEtiquetaJSON(const std::map<std::string, std::vector<Tarefa>>& agrupadas);

        // Exporta relatório de tarefas agrupadas por etiqueta para CSV
        static std::string exportarTarefasPorEtiquetaCSV(const std::map<std::string, std::vector<Tarefa>>& agrupadas);


    };

} // namespace afazer::dominio
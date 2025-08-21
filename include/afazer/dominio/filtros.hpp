#pragma once
#include "afazer/dominio/tarefa.hpp"
#include <vector>
#include <string>
#include <functional>

namespace afazer::dominio {

class Filtros {
public:
    // Filtra tarefas por status
    static std::vector<Tarefa> porStatus(const std::vector<Tarefa>& tarefas, Status status);

    // Filtra tarefas por prioridade
    static std::vector<Tarefa> porPrioridade(const std::vector<Tarefa>& tarefas, Prioridade prioridade);

    // Filtra tarefas por etiqueta
    static std::vector<Tarefa> porEtiqueta(const std::vector<Tarefa>& tarefas, const std::string& etiqueta);
};

} // namespace afazer::dominio
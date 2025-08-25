#pragma once
#include "afazer/dominio/tarefa.hpp"
#include <string>
#include <vector>

namespace afazer::dominio {

class Notificador {
public:
    // Gera notificações para tarefas atrasadas
    static std::vector<std::string> notificacoesAtrasadas(const std::vector<Tarefa>& tarefas);

    // Gera notificações para tarefas próximas de vencer
    static std::vector<std::string> notificacoesProximasDeVencer(const std::vector<Tarefa>& tarefas, int dias);
};

} // namespace afazer::dominio
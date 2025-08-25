#include "afazer/dominio/notificador.hpp"

namespace afazer::dominio {

std::vector<std::string> Notificador::notificacoesAtrasadas(const std::vector<Tarefa>& tarefas) {
    std::vector<std::string> notificacoes;

    for (const auto& tarefa : tarefas) {
        if (tarefa.estaAtrasada()) {
            notificacoes.push_back("A tarefa \"" + tarefa.titulo() + "\" está atrasada!");
        }
    }

    return notificacoes;
}

std::vector<std::string> Notificador::notificacoesProximasDeVencer(const std::vector<Tarefa>& tarefas, int dias) {
    std::vector<std::string> notificacoes;

    for (const auto& tarefa : tarefas) {
        if (tarefa.estaProximaDeVencer(dias)) {
            notificacoes.push_back("A tarefa \"" + tarefa.titulo() + "\" está próxima de vencer!");
        }
    }

    return notificacoes;
}

} // namespace afazer::dominio
#include "afazer/dominio/notificador.hpp"
#include "afazer/dominio/logger.hpp"

namespace afazer::dominio {

Notificador::Notificador(const Configuracoes& configuracoes) : configuracoes_(configuracoes) {}

std::vector<std::string> Notificador::notificacoesAtrasadas(const std::vector<Tarefa>& tarefas) const {
    std::vector<std::string> notificacoes;

    for (const auto& tarefa : tarefas) {
        if (tarefa.estaAtrasada()) {
            std::string mensagem = "A tarefa \"" + tarefa.titulo() + "\" está atrasada!";
            notificacoes.push_back(mensagem);

            // Registra no log
            Logger logger("sistema.log");
            logger.registrarComNivel("WARNING", mensagem);
        }
    }

    return notificacoes;
}

std::vector<std::string> Notificador::notificacoesProximasDeVencer(const std::vector<Tarefa>& tarefas) const {
    std::vector<std::string> notificacoes;

    int dias = std::stoi(configuracoes_.obterComPadrao("dias_notificacao", "3"));

    for (const auto& tarefa : tarefas) {
        if (tarefa.estaProximaDeVencer(dias)) {
            std::string mensagem = "A tarefa \"" + tarefa.titulo() + "\" está próxima de vencer!";
            notificacoes.push_back(mensagem);

            // Registra no log
            Logger logger("sistema.log");
            logger.registrarComNivel("INFO", mensagem);
        }
    }

    return notificacoes;
}

} // namespace afazer::dominio
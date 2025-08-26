#pragma once
#include "afazer/dominio/tarefa.hpp"
#include "afazer/dominio/configuracoes.hpp"
#include <string>
#include <vector>

namespace afazer::dominio {

class Notificador {
public:

    // Construtor que aceita configurações
    explicit Notificador(const Configuracoes& configuracoes);

    // Gera notificações para tarefas atrasadas
    std::vector<std::string> notificacoesAtrasadas(const std::vector<Tarefa>& tarefas) const;
    
    // Gera notificações para tarefas próximas de vencer
    std::vector<std::string> notificacoesProximasDeVencer(const std::vector<Tarefa>& tarefas) const;    
    
private:
    const Configuracoes& configuracoes_;
};

} // namespace afazer::dominio
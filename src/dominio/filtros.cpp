#include "afazer/dominio/filtros.hpp"
#include <algorithm>

namespace afazer::dominio {

std::vector<Tarefa> Filtros::porStatus(const std::vector<Tarefa>& tarefas, Status status) {
    std::vector<Tarefa> resultado;
    std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(resultado),
                 [status](const Tarefa& tarefa) { return tarefa.status() == status; });
    return resultado;
}

std::vector<Tarefa> Filtros::porPrioridade(const std::vector<Tarefa>& tarefas, Prioridade prioridade) {
    std::vector<Tarefa> resultado;
    std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(resultado),
                 [prioridade](const Tarefa& tarefa) { return tarefa.prioridade() == prioridade; });
    return resultado;
}

std::vector<Tarefa> Filtros::porEtiqueta(const std::vector<Tarefa>& tarefas, const std::string& etiqueta) {
    std::vector<Tarefa> resultado;
    std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(resultado),
                 [&etiqueta](const Tarefa& tarefa) {
                     const auto& etiquetas = tarefa.etiquetas();
                     return std::find(etiquetas.begin(), etiquetas.end(), etiqueta) != etiquetas.end();
                 });
    return resultado;
}

} // namespace afazer::dominio
#include "afazer/dominio/relatorios.hpp"
#include <nlohmann/json.hpp>
#include <algorithm>

namespace afazer::dominio {

    using json = nlohmann::json;

    std::vector<Tarefa> Relatorios::tarefasPorStatus(const std::vector<Tarefa>& tarefas, Status status) {
        std::vector<Tarefa> resultado;
        std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(resultado), 
                        [status] (const Tarefa& tarefa) {return tarefa.status() == status;});
        return resultado; 
    }

    std::map<Status, int> Relatorios::contagemPorStatus(const std::vector<Tarefa>& tarefas) {
        std::map<Status, int> contagem;
        for (const auto& tarefa : tarefas) {
            contagem[tarefa.status()]++;
        }
        return contagem;
    }

    std::map<Prioridade, int> Relatorios::contagemPorPrioridade(const std::vector<Tarefa>& tarefas) {
        std::map<Prioridade, int> contagem;
        for (const auto& tarefa : tarefas) {
            contagem[tarefa.prioridade()]++;
        }
        return contagem;
    }

    std::vector<Tarefa> Relatorios::tarefasPorPrioridade(const std::vector<Tarefa>& tarefas, Prioridade prioridade) {
        std::vector<Tarefa> resultado;
        std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(resultado),
                    [prioridade](const Tarefa& tarefa) { return tarefa.prioridade() == prioridade; });
        return resultado;
    }

    std::map<std::string, std::vector<Tarefa>> Relatorios::tarefasPorEtiqueta(const std::vector<Tarefa>& tarefas) {
        std::map<std::string, std::vector<Tarefa>> agrupadas;
        for (const auto& tarefa : tarefas) {
            for (const auto& etiqueta : tarefa.etiquetas()) {
                agrupadas[etiqueta].push_back(tarefa);
            }
        }
        return agrupadas;
    }
    
    std::vector<Tarefa> Relatorios::tarefasAtrasadas(const std::vector<Tarefa>& tarefas) {
        std::vector<Tarefa> atrasadas;
        std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(atrasadas),
                    [](const Tarefa& tarefa) { return tarefa.estaAtrasada(); });
        return atrasadas;
    }

    std::vector<Tarefa> Relatorios::tarefasProximasDeVencer(const std::vector<Tarefa>& tarefas, int dias) {
        std::vector<Tarefa> proximas;
        std::copy_if(tarefas.begin(), tarefas.end(), std::back_inserter(proximas),
                    [dias](const Tarefa& tarefa) { return tarefa.estaProximaDeVencer(dias); });
        return proximas;
    }
    
    std::string Relatorios::exportarContagemPorStatusJSON(const std::map<Status, int>& contagem) {
        json jContagem;
        for (const auto& [status, quantidade] : contagem) {
            jContagem[std::to_string(static_cast<int>(status))] = quantidade;
        }
        return jContagem.dump(4); // Formata o JSON com indentação de 4 espaços
    }

    std::string Relatorios::exportarContagemPorPrioridadeJSON(const std::map<Prioridade, int>& contagem) {
        json jContagem;
        for (const auto& [prioridade, quantidade] : contagem) {
            jContagem[std::to_string(static_cast<int>(prioridade))] = quantidade;
        }
        return jContagem.dump(4); // Formata o JSON com indentação de 4 espaços
    }

    std::string Relatorios::exportarTarefasPorEtiquetaJSON(const std::map<std::string, std::vector<Tarefa>>& agrupadas) {
        json jAgrupadas;
        for (const auto& [etiqueta, tarefas] : agrupadas) {
            json jTarefas = json::array();
            for (const auto& tarefa : tarefas) {
                jTarefas.push_back({
                    {"id", tarefa.id()},
                    {"titulo", tarefa.titulo()},
                    {"descricao", tarefa.descricao().value_or("")},
                    {"status", static_cast<int>(tarefa.status())},
                    {"prioridade", static_cast<int>(tarefa.prioridade())},
                    {"dataCriacao", std::chrono::duration_cast<std::chrono::seconds>(
                                    tarefa.dataCriacao().time_since_epoch())
                                    .count()},
                    {"etiquetas", tarefa.etiquetas()}
                });
            }
            jAgrupadas[etiqueta] = jTarefas;
        }
        return jAgrupadas.dump(4); // Formata o JSON com indentação de 4 espaços
    }

    std::string Relatorios::exportarTarefasPorEtiquetaCSV(const std::map<std::string, std::vector<Tarefa>>& agrupadas) {
        std::ostringstream oss;

        // Cabeçalho
        oss << "Etiqueta,ID,Título,Descrição,Status,Prioridade,Data de Criação,Etiquetas\n";

        for (const auto& [etiqueta, tarefas] : agrupadas) {
            for (const auto& tarefa : tarefas) {
                oss << etiqueta << ","
                    << tarefa.id() << ","
                    << "\"" << tarefa.titulo() << "\","
                    << "\"" << tarefa.descricao().value_or("") << "\","
                    << static_cast<int>(tarefa.status()) << ","
                    << static_cast<int>(tarefa.prioridade()) << ","
                    << std::chrono::duration_cast<std::chrono::seconds>(
                        tarefa.dataCriacao().time_since_epoch())
                        .count()
                    << ",\"";

                for (size_t i = 0; i < tarefa.etiquetas().size(); ++i) {
                    oss << tarefa.etiquetas()[i];
                    if (i < tarefa.etiquetas().size() - 1) {
                        oss << ";";
                    }
                }
                oss << "\"\n";
            }
        }
        return oss.str();
    }

} // namespace dominio
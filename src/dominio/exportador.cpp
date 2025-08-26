#include "afazer/dominio/exportador.hpp"
#include "afazer/dominio/tarefa.hpp"
#include <nlohmann/json.hpp>
#include <sstream>

namespace afazer::dominio {

    using json = nlohmann::json;

    Exportador::Exportador(const Configuracoes& configuracoes) : configuracoes_(configuracoes) {}

    std::string Exportador::exportar(const std::vector<Tarefa>& tarefas) const {
        // Obtém o formato de exportação configurado
        std::string formato = configuracoes_.obterComPadrao("formato_exportacao", "json");

        if (formato == "json") {
            return paraJSON(tarefas);
        } else if (formato == "csv") {
            return paraCSV(tarefas);
        } else {
            throw std::runtime_error("Formato de exportação desconhecido: " + formato);
        }
    }

    std::string Exportador::paraJSON(const std::vector<Tarefa>& tarefas) {
        json jTarefas = json::array();

        for (const auto& tarefa : tarefas) {

            // Converte dataConclusao para um valor JSON-friendly
            json dataConclusaoJson = tarefa.dataConclusao()
                                     ? json(std::chrono::duration_cast<std::chrono::seconds>(
                                                tarefa.dataConclusao()->time_since_epoch())
                                                .count())
                                     : json(nullptr);

            json jTarefa = {
                {"id", tarefa.id()},
                {"titulo", tarefa.titulo()},
                {"descricao", tarefa.descricao().value_or("")},
                {"status", static_cast<int>(tarefa.status())},
                {"prioridade", static_cast<int>(tarefa.prioridade())},
                {"dataCriacao", std::chrono::duration_cast<std::chrono::seconds>(tarefa.dataCriacao().time_since_epoch()).count()},
                {"dataConclusao", dataConclusaoJson}, // Usa std::optional
                {"etiquetas", tarefa.etiquetas()}
            };
            jTarefas.push_back(jTarefa);
        }
        return jTarefas.dump(4); // Formato JSON com indentação 4 espaços
    }

    std::string Exportador::paraCSV(const std::vector<Tarefa>& tarefas) {
        std::ostringstream oss;

        // Cabeçalho
        oss << "ID,Título,Descrição,Status,Prioridade,Data da Crianção,Data da Conclusão,Etiquetas\n";
        for (const auto& tarefa : tarefas){
            oss << tarefa.id() << ","
            << "\"" << tarefa.titulo() << "\","
            << "\"" << tarefa.descricao().value_or("") << "\","
            << static_cast<int>(tarefa.status()) << ","
            << static_cast<int>(tarefa.prioridade()) << ","
            << std::chrono::duration_cast<std::chrono::seconds>(
                   tarefa.dataCriacao().time_since_epoch())
                   .count()
            << ",";

            if (tarefa.dataConclusao()) {
                oss << std::chrono::duration_cast<std::chrono::seconds>(tarefa.dataConclusao()->time_since_epoch()).count();
            }
            else {
                oss << "null";
            }

            oss << ",\"";

            for(size_t i = 0; i < tarefa.etiquetas().size(); ++i) {
                oss << tarefa.etiquetas()[i];
                if( i < tarefa.etiquetas().size() -1 ) {
                    oss << ";";
                }
            }
        }
        return oss.str();
    }

std::vector<Tarefa> Exportador::deJSON(const std::string& jsonString) {
    json jTarefas = json::parse(jsonString);
    std::vector<Tarefa> tarefas;

        for (const auto& jTarefa : jTarefas) {
            std::string titulo = jTarefa.at("titulo").get<std::string>();
            std::string descricao = jTarefa.at("descricao").get<std::string>();
            Prioridade prioridade = static_cast<Prioridade>(jTarefa.at("prioridade").get<int>());

            Tarefa tarefa(titulo, descricao, prioridade);

            // Adiciona etiquetas
            for (const auto& etiqueta : jTarefa.at("etiquetas").get<std::vector<std::string>>()) {
                tarefa.adicionarEtiqueta(etiqueta);
            }

            tarefas.push_back(std::move(tarefa));
        }

        return tarefas;
    }

std::vector<Tarefa> Exportador::deCSV(const std::string& csvString) {
    std::vector<Tarefa> tarefas;
    std::istringstream iss(csvString);
    std::string linha;

    // Ignora o cabeçalho
    std::getline(iss, linha);

    while (std::getline(iss, linha)) {
        std::istringstream linhaStream(linha);
        std::string campo;

        std::getline(linhaStream, campo, ',');
        std::string titulo = campo;

        std::getline(linhaStream, campo, ',');
        std::string descricao = campo;

        std::getline(linhaStream, campo, ',');
        Prioridade prioridade = static_cast<Prioridade>(std::stoi(campo));

        Tarefa tarefa(titulo, descricao, prioridade);

        tarefas.push_back(std::move(tarefa));
    }

    return tarefas;
}

} // namespace afazer::dominio
#include "afazer/dominio/exportador.hpp"
#include "afazer/dominio/tarefa.hpp"
#include <nlohmann/json.hpp>
#include <sstream>

namespace afazer::dominio {

    using json = nlohmann::json;

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
        // Extrai os campos do JSON
        std::string titulo = jTarefa.at("titulo").get<std::string>();
        std::string descricao = jTarefa.at("descricao").get<std::string>();
        Prioridade prioridade = static_cast<Prioridade>(jTarefa.at("prioridade").get<int>());
        auto dataConclusao = jTarefa.at("dataConclusao").is_null()
                                 ? std::nullopt
                                 : std::optional<std::chrono::system_clock::time_point>(
                                       std::chrono::system_clock::time_point(
                                           std::chrono::seconds(jTarefa.at("dataConclusao").get<long int>())));

        std::vector<std::string> etiquetas = jTarefa.at("etiquetas").get<std::vector<std::string>>();

        // Cria a tarefa
        Tarefa tarefa(titulo, descricao, prioridade);

        // Adiciona etiquetas
        for (const auto& etiqueta : etiquetas) {
            tarefa.adicionarEtiqueta(etiqueta);
        }

        // Define o status e a data de conclusão, se necessário
        if (jTarefa.at("status").get<int>() == static_cast<int>(Status::Concluida)) {
            tarefa.concluir();
        } else if (jTarefa.at("status").get<int>() == static_cast<int>(Status::Cancelada)) {
            tarefa.cancelar();
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

        // Extrai os campos do CSV
        std::getline(linhaStream, campo, ',');
        std::string titulo = campo;

        std::getline(linhaStream, campo, ',');
        std::string descricao = campo;

        std::getline(linhaStream, campo, ',');
        Prioridade prioridade = static_cast<Prioridade>(std::stoi(campo));

        std::getline(linhaStream, campo, ',');
        auto dataConclusao = (campo != "null")
                                 ? std::optional<std::chrono::system_clock::time_point>(
                                       std::chrono::system_clock::time_point(
                                           std::chrono::seconds(std::stol(campo))))
                                 : std::nullopt;

        std::getline(linhaStream, campo, ',');
        std::vector<std::string> etiquetas;
        std::istringstream etiquetasStream(campo);
        std::string etiqueta;
        while (std::getline(etiquetasStream, etiqueta, ';')) {
            etiquetas.push_back(etiqueta);
        }

        // Cria a tarefa
        Tarefa tarefa(titulo, descricao, prioridade);

        // Adiciona etiquetas
        for (const auto& etiqueta : etiquetas) {
            tarefa.adicionarEtiqueta(etiqueta);
        }

        tarefas.push_back(std::move(tarefa));
    }

    return tarefas;
}

} // namespace afazer::dominio
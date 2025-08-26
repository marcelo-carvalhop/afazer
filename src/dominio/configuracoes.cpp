#include "afazer/dominio/configuracoes.hpp"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace afazer::dominio {

using json = nlohmann::json;

void Configuracoes::definir(const std::string& chave, const std::string& valor) {
    configuracoes_[chave] = valor;
}

std::string Configuracoes::obter(const std::string& chave) const {
    auto it = configuracoes_.find(chave);
    if (it == configuracoes_.end()) {
        throw std::runtime_error("Configuração não encontrada: " + chave);
    }
    return it->second;
}

std::string Configuracoes::obterComPadrao(const std::string& chave, const std::string& valorPadrao) const {
    auto it = configuracoes_.find(chave);
    if (it == configuracoes_.end()) {
        return valorPadrao;
    }
    return it->second;
}

void Configuracoes::salvarEmArquivo(const std::string& caminhoArquivo) const {
    json jConfiguracoes(configuracoes_);

    std::ofstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo para salvar as configurações: " + caminhoArquivo);
    }

    arquivo << jConfiguracoes.dump(4); // Salva o JSON com indentação de 4 espaços
    arquivo.close();
}

void Configuracoes::carregarDeArquivo(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo para carregar as configurações: " + caminhoArquivo);
    }

    json jConfiguracoes;
    arquivo >> jConfiguracoes;
    arquivo.close();

    configuracoes_ = jConfiguracoes.get<std::map<std::string, std::string>>();
}

} // namespace afazer::dominio
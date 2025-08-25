#include "afazer/dominio/configuracoes.hpp"
#include <stdexcept>

namespace afazer::dominio {

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

} // namespace afazer::dominio
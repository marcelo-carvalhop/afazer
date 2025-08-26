#pragma once
#include <string>
#include <map>

namespace afazer::dominio {

class Configuracoes {
public:
    // Define uma configuração
    void definir(const std::string& chave, const std::string& valor);

    // Obtém o valor de uma configuração
    std::string obter(const std::string& chave) const;

    // Obtém o valor de uma configuração com um valor padrão
    std::string obterComPadrao(const std::string& chave, const std::string& valorPadrao) const;

    // Salva as configurações em um arquivo
    void salvarEmArquivo(const std::string& caminhoArquivo) const;

    // Carrega as configurações de um arquivo
    void carregarDeArquivo(const std::string& caminhoArquivo);

private:
    std::map<std::string, std::string> configuracoes_;
};

} // namespace afazer::dominio
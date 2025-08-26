#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <tuple>
#include <vector>

namespace afazer::dominio {

class Logger {
public:
    // Construtor que aceita o caminho do arquivo de log
    explicit Logger(const std::string& caminhoArquivo);

    // Registra uma mensagem no log
    void registrar(const std::string& mensagem);

    // Define o nível de log (INFO, WARNING, ERROR)
    void registrarComNivel(const std::string& nivel, const std::string& mensagem);

    // Exporta os logs em JSON
    std::string exportarJSON();

    // Exporta os logs em CSV
    std::string exportarCSV();

    // Filtra os logs por nível
    std::vector<std::tuple<std::string, std::string, std::string>> filtrarPorNivel(const std::string& nivel);

    // Filtra os logs por período de tempo
    std::vector<std::tuple<std::string, std::string, std::string>> filtrarPorPeriodo(const std::string& inicio, const std::string& fim);

private:
    std::string caminhoArquivo_; // Caminho do arquivo de log
    std::ofstream arquivoLog_;
    std::mutex mutex_;           // Mutex para garantir acesso thread-safe}

};

} // namespace afazer::dominio
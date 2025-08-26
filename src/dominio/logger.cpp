#include "afazer/dominio/logger.hpp"
#include <nlohmann/json.hpp>
#include <filesystem> // Para manipular diretórios
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>


namespace afazer::dominio {

using json = nlohmann::json; // Alias para simplificar o uso do JSON

Logger::Logger(const std::string& caminhoArquivo) : caminhoArquivo_(caminhoArquivo) {
    // Garante que o diretório do arquivo de log exista
    std::filesystem::path caminho(caminhoArquivo);
    std::filesystem::create_directories(caminho.parent_path());

    std::ofstream arquivo(caminhoArquivo, std::ios::app); // Abre o arquivo em modo append
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo);
    }
}

void Logger::registrar(const std::string& mensagem) {
    std::lock_guard<std::mutex> lock(mutex_); // Bloqueia o mutex para acesso thread-safe

    auto agora = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&agora));

    std::ofstream arquivo(caminhoArquivo_, std::ios::app);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo_);
    }

    arquivo << "[" << buffer << "] " << mensagem << std::endl;
}

void Logger::registrarComNivel(const std::string& nivel, const std::string& mensagem) {
    registrar("[" + nivel + "] " + mensagem);
}

std::string Logger::exportarJSON() {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ifstream arquivo(caminhoArquivo_);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo_);
    }

    json jLogs = json::array();
    std::string linha;

    while (std::getline(arquivo, linha)) {
        jLogs.push_back(linha);
    }

    return jLogs.dump(4); // Retorna o JSON formatado
}

std::string Logger::exportarCSV()  {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ifstream arquivo(caminhoArquivo_);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo_);
    }

    std::ostringstream oss;
    oss << "Timestamp,Nível,Mensagem\n";

    std::string linha;
    while (std::getline(arquivo, linha)) {
        // Exemplo de linha: [2025-08-26 14:00:00] [INFO] Mensagem de teste
        auto timestampFim = linha.find("] ");
        auto nivelFim = linha.find("] ", timestampFim + 2);

        std::string timestamp = linha.substr(1, timestampFim - 1);
        std::string nivel = linha.substr(timestampFim + 3, nivelFim - timestampFim - 3);
        std::string mensagem = linha.substr(nivelFim + 2);

        oss << timestamp << "," << nivel << ",\"" << mensagem << "\"\n";
    }

    return oss.str();
}

std::vector<std::tuple<std::string, std::string, std::string>> Logger::filtrarPorNivel(const std::string& nivel) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ifstream arquivo(caminhoArquivo_);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo_);
    }

    std::vector<std::tuple<std::string, std::string, std::string>> logsFiltrados;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        auto timestampFim = linha.find("] ");
        auto nivelFim = linha.find("] ", timestampFim + 2);

        std::string timestamp = linha.substr(1, timestampFim - 1);
        std::string nivelAtual = linha.substr(timestampFim + 3, nivelFim - timestampFim - 3);
        std::string mensagem = linha.substr(nivelFim + 2);

        if (nivelAtual == nivel) {
            logsFiltrados.emplace_back(timestamp, nivelAtual, mensagem);
        }
    }

    return logsFiltrados;
}

std::vector<std::tuple<std::string, std::string, std::string>> Logger::filtrarPorPeriodo(const std::string& inicio, const std::string& fim) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ifstream arquivo(caminhoArquivo_);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo de log: " + caminhoArquivo_);
    }

    std::vector<std::tuple<std::string, std::string, std::string>> logsFiltrados;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        auto timestampFim = linha.find("] ");
        auto nivelFim = linha.find("] ", timestampFim + 2);

        std::string timestamp = linha.substr(1, timestampFim - 1);
        std::string nivel = linha.substr(timestampFim + 3, nivelFim - timestampFim - 3);
        std::string mensagem = linha.substr(nivelFim + 2);

        if (timestamp >= inicio && timestamp <= fim) {
            logsFiltrados.emplace_back(timestamp, nivel, mensagem);
        }
    }

    return logsFiltrados;
}

} // namespace afazer::dominio
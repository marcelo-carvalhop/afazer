#include "afazer/dominio/tarefa.hpp"
#include <algorithm>
#include <stdexcept>

namespace afazer::dominio{

    // Inicializa o contador de IDs
    static int contadorId = 0;

// Construtor
Tarefa::Tarefa(std::string titulo, std::optional<std::string> descricao, Prioridade prioridade)
    : id_(gerarID()), titulo_(std::move(titulo)), descricao_(std::move(descricao)), status_(Status::Pendente),
      prioridade_(prioridade), dataCriacao_(std::chrono::system_clock::now()), periodicidade_(Periodicidade::Nenhuma) {
    if (titulo_.empty()) {
        throw std::invalid_argument("O título da tarefa não pode ser vazio.");
    }
}

    //Getters
    int Tarefa::id() const {
        return id_;
    }

    const std::string& Tarefa::titulo() const {
        return titulo_;
    }

    const std::optional<std::string>& Tarefa::descricao() const {
        return descricao_;
    }

    Status Tarefa::status() const {
        return status_;
    }

    Prioridade Tarefa::prioridade() const {
        return prioridade_;
    }

    const std::chrono::system_clock::time_point& Tarefa::dataCriacao() const {
        return dataCriacao_;
    }

    const std::vector<std::string>& Tarefa::etiquetas() const {
        return etiquetas_;
    }

    Periodicidade Tarefa::periodicidade() const {
    return periodicidade_;  
    }

    const std::optional<std::chrono::system_clock::time_point>& Tarefa::proximaOcorrencia() const {
        return proximaOcorrencia_;
    }

    const std::optional<std::chrono::system_clock::time_point>& Tarefa::dataVencimento() const {
        return dataVencimento_;
    }

    const std::optional<std::chrono::system_clock::time_point>& Tarefa::dataConclusao() const {
        return dataConclusao_;
    }

    // Métodos de negócio

    void Tarefa::concluir() {
        if (status_ == Status::Concluida){
            throw std::logic_error("A tarefa já está concluída.");
        }
        status_ = Status::Concluida;
        dataConclusao_ = std::chrono::system_clock::now();
    }

    void Tarefa::cancelar() {
        if (status_ == Status::Cancelada){
            throw std::logic_error("A tarefa já está concelada.");
        }
        status_ = Status::Cancelada;
        dataConclusao_ = std::nullopt;
    }

    void Tarefa::adicionarEtiqueta(const std::string& etiqueta) {
        if (std::find(etiquetas_.begin(), etiquetas_.end(), etiqueta) == etiquetas_.end()) {
            etiquetas_.push_back(etiqueta);
        }
    }

    void Tarefa::removerEtiqueta(const std::string& etiqueta) {
        etiquetas_.erase(std::remove(etiquetas_.begin(), etiquetas_.end(), etiqueta), etiquetas_.end());
    }

    void Tarefa::definirPeriodicidade(Periodicidade periodicidade) {
        periodicidade_ = periodicidade;
        calcularProximaOcorrencia();
    }

    void Tarefa::calcularProximaOcorrencia() {
        if (periodicidade_ == Periodicidade::Nenhuma) {
            proximaOcorrencia_ = std::nullopt;
            return;
        }

        auto agora = std::chrono::system_clock::now();
        if (!dataConclusao_) {
            proximaOcorrencia_ = agora;
            return;
        }

        switch (periodicidade_) {
            case Periodicidade::Diaria:
                proximaOcorrencia_ = *dataConclusao_ + std::chrono::hours(24);
                break;
            case Periodicidade::Semanal:
                proximaOcorrencia_ = *dataConclusao_ + std::chrono::hours(24 * 7);
                break;
            case Periodicidade::Mensal:
                proximaOcorrencia_ = *dataConclusao_ + std::chrono::hours(24 * 30);
                break;
            default:
                proximaOcorrencia_ = std::nullopt;
                break;
        }
    }

    void Tarefa::definirDataVencimento(const std::chrono::system_clock::time_point& dataVencimento) {
        dataVencimento_ = dataVencimento;
    }

    bool Tarefa::estaAtrasada() const {
        if (!dataVencimento_ || status_ == Status::Concluida || status_ == Status::Cancelada) {
            return false;
        }
        return std::chrono::system_clock::now() > *dataVencimento_;
    }

    bool Tarefa::estaProximaDeVencer(int dias) const {
        if (!dataVencimento_ || status_ == Status::Concluida || status_ == Status::Cancelada) {
            return false;
        }
        auto agora = std::chrono::system_clock::now();
        auto limite = agora + std::chrono::hours(24 * dias);
        return *dataVencimento_ <= limite && *dataVencimento_ > agora;
    }

    //Gera um ID único
    int Tarefa::gerarID() {
        return ++contadorId;
    }
} // namespace afazer::dominio
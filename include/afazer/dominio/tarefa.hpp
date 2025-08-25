#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace afazer::dominio {

    enum class Status {
        Pendente,
        Concluida,
        Cancelada
    };

    enum class Prioridade {
        Baixa,
        Media,
        Alta
    };

    enum class Periodicidade {
        Nenhuma,
        Diaria,
        Semanal,
        Mensal
    };

    class Tarefa {
    public:
        //Construtor
        Tarefa(std::string titulo, std::optional<std::string> descricao = std::nullopt, Prioridade prioridade = Prioridade::Media);

        ///Getters
        int id() const;
        const std::string& titulo() const;
        const std::optional<std::string>& descricao() const;
        Status status() const;
        Prioridade prioridade() const;
        const std::chrono::system_clock::time_point& dataCriacao() const;
        const std::optional<std::chrono::system_clock::time_point>& dataConclusao() const;
        const std::vector<std::string>& etiquetas() const;
        Periodicidade periodicidade() const;
        const std::optional<std::chrono::system_clock::time_point>& proximaOcorrencia() const;
        const std::optional<std::chrono::system_clock::time_point>& dataVencimento() const;

        //Métodos de negócio
        void concluir();
        void cancelar();
        void adicionarEtiqueta(const std::string& etiqueta);
        void removerEtiqueta(const std::string& etiqueta);
        void definirPeriodicidade(Periodicidade periodicidade);
        void calcularProximaOcorrencia();
        void definirDataVencimento(const std::chrono::system_clock::time_point& dataVencimento);
        bool estaAtrasada() const;
        bool estaProximaDeVencer(int dias) const;    
    
    private:
        int id_;
        std::string titulo_;
        std::optional<std::string> descricao_;
        Status status_;
        Prioridade prioridade_;
        std::chrono::system_clock::time_point dataCriacao_;
        std::optional<std::chrono::system_clock::time_point> dataConclusao_;
        std::vector<std::string> etiquetas_;
        Periodicidade periodicidade_;
        std::optional<std::chrono::system_clock::time_point> proximaOcorrencia_;
        std::optional<std::chrono::system_clock::time_point> dataVencimento_;
        
        //Gera um ID único para cada Tarefa
        static int gerarID();
    };
} //namespace afazer::dominio
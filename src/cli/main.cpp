#include "afazer/dominio/tarefa.hpp"
#include <iostream>

int main() {
    using namespace afazer::dominio;

    try {
        Tarefa tarefa("Estudar C++", "Revisar RAII e smart pointers", Prioridade::Alta);
        std::cout << "Tarefa criada com sucesso: " << tarefa.titulo() << "\n";
        tarefa.concluir();
        std::cout << "Tarefa concluída com sucesso!\n";
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
    }

    return 0;
}
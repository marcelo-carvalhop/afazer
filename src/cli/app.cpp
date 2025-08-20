/* src/cli/app.hpp */
#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include "afazer/cli/app.hpp"

#ifndef AFAZER_VERSION
#define AFAZER_VERSION "0.0.0"
#endif

namespace {

  constexpr std::string_view AJUDA = R"(aFazer - Gerenciador de tarefas (versão incial)
Uso:
 afazer [opções] [comando]

Opções:
 --ajuda     Mostra esta ajuda e sai.
 --versao    Mostra a versão e sai.

Comandos (pré-visualização; serão implementados nos próximos capítulos):
 inicar, usar, adicionar, listar, mostrar, editar, concluir, reabir, excluir, exportar, importar

Exemplos:
 afazer --ajuda
 afazer --versao
)";

  bool tem_flag(int argc, char** argv, std::string_view flag) {
    for (int i = 1; i < argc; ++i) {
      if (std::string_view(argv[i]) == flag) return true;
    }
    return false;
  }

}// namespace

namespace afazer::cli {

  int App::executar(int argc, char** argv) {
    // Sem argumentos: mostra ajuda rápida
    if (argc <= 1){
      std::cout << AJUDA;
      return 0;
    }

    // Opções básicas
    if (tem_flag(argc, argv, "--ajuda")) {
      std::cout << AJUDA;
      return 0;
    }

    if (tem_flag(argc, argv, "--versao")) {
      std::cout << "aFazer " << AFAZER_VERSION << "\n";
      return 0;
    }

    // Por ora, qualquer outra coisa é inválida nesta versão inciial
    std::cerr << "Argumento/comando não reconhecido. Use --ajuda para ver as opções.\n";
    // Código 6 = argumento/validação inválida (padrão do projeto)
    return 6;
  }
} // namespace afazer::cli

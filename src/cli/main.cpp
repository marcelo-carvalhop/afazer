/* src/cli/main  */
#include <iostream>
#include "afazer/cli/app.hpp"

int main(int argc, char** argv){
  try {
    return afazer::cli::App{}.executar(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << "Erro inesperado: " << e.what() << "\n";
    return 1;
  }
}

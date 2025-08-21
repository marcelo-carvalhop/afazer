#pragma once
#include <string>
#include <chrono>

namespace afazer::util {
    
    std::string timestampAtual();
    std::string formatarDuracao(std::chrono::seconds duracao);

} // namespace afazer::util
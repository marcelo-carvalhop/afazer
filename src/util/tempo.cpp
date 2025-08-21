#include "afazer/util/tempo.hpp"
#include <iomanip>
#include <sstream>

namespace afazer::util {

    std::string timestampAtual() {
        auto agora = std::chrono::system_clock::now();
        auto tempo = std::chrono::system_clock::to_time_t(agora);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&tempo), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string formatarDuracao(std::chrono::seconds duracao) {
        auto horas = std::chrono::duration_cast<std::chrono::hours>(duracao);
        duracao -= horas;
        auto minutos = std::chrono::duration_cast<std::chrono::minutes>(duracao);
        duracao -= minutos;
        std::ostringstream oss;
        oss << horas.count() << "h " << minutos.count() << "m " << duracao.count() << "s";
    }

} // Namespace afazer::util
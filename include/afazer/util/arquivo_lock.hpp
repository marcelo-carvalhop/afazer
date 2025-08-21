#pragma once
#include <string>
#include <system_error>

namespace afazer::util {

class ArquivoLock {
public:
    explicit ArquivoLock(const std::string& caminho);
    ~ArquivoLock();

    // Proíbe cópia
    ArquivoLock(const ArquivoLock&) = delete;
    ArquivoLock& operator=(const ArquivoLock&) = delete;

    // Permite movimentação
    ArquivoLock(ArquivoLock&&) noexcept;
    ArquivoLock& operator=(ArquivoLock&&) noexcept;

    // Verifica se o lock foi adquirido
    bool adquirido() const;

private:
    std::string caminho_;
    int handle_; // Descritor de arquivo (Linux) ou HANDLE (Windows)
    bool adquirido_;
};

} // namespace afazer::util
#include "afazer/util/arquivo_lock.hpp"
#include <fcntl.h> // Para open/flock no Linux
#include <unistd.h> // Para close no linux
#include <sys/file.h> // Para a função flock
#include <stdexcept>
#include <system_error>

#ifdef _WIN32
#include <windows.h>
#endif

namespace afazer::util {

ArquivoLock::ArquivoLock(const std::string& caminho)
    : caminho_(caminho), handle_(-1), adquirido_(false) {
#ifdef _WIN32
        handle_ = CreateFileA(
            caminho.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0, // Sem compartilhamento
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        if (hande_ == INVALID_HANDLE_VALUE){
            throw std::system_error(GetLastError(), std::system_category(), "Falha ao abrir arquivo para lock.");    
        }
        adquirido_ = true;
#else
        handle_ = open(caminho.c_str(), O_RDWR | O_CREAT, 0666);
        if (handle_ == -1){
            throw std::system_error (errno, std::system_category(), "Falha ao abrir arquivo para lock.");
        }
        adquirido_ = true;
#endif
    }

ArquivoLock::ArquivoLock(ArquivoLock&& outro) noexcept
    : caminho_(std::move(outro.caminho_)), handle_(outro.handle_),
    adquirido_(outro.adquirido_) {
        outro.handle_ = -1;
        outro.adquirido_ = false;
    }

ArquivoLock::~ArquivoLock() {
    if (adquirido_) {
#ifdef _WIN32
        CloseHandle(handle_);
#else
        flock(handle_, LOCK_UN); // Libera o lock
        close(handle_);          // Fecha o arquivo
#endif
    }
}

ArquivoLock& ArquivoLock::operator=(ArquivoLock&& outro) noexcept {
    if (this != &outro) {
        if(adquirido_) {
#ifdef _WIN32
            CloseHandle(handle_);
#else
            if (flock(handle_, LOCK_UN) == -1){
                perror("Erro ao liberar o lock");
            }
            close(handle_);
#endif
        }
        caminho_ = std::move(outro.caminho_);
        handle_ = outro.handle_;
        adquirido_ = outro.adquirido_;
        outro.handle_ = -1;
        outro.adquirido_ = false;
    }
    return *this;
}

bool ArquivoLock::adquirido() const {
    return adquirido_;
}

} // namespace afazer::util
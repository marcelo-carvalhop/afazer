cmake_minimum_required(VERSION 3.20)

# Nome e versão do projeto
project(aFazer LANGUAGES CXX VERSION 0.1.0)

# Opções
option(AFAZER_BUILD_TESTES "Construir testes" ON)
option(AFAZER_BUILD_TUI "Construir TUI (ncurses/PDCurses)" OFF)
option(AFAZER_WARNING_AS_ERRORS "Tratar warnings como erros" OFF)

# Padrão de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Exporta compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Flags por compilador
if(MSVC)
  add_compile_options(/W4 /permissive- /EHsc)
  if(AFAZER_WARNING_AS_ERRORS)
    add_compile_options(/WX)
  endif()
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
  if(AFAZER_WARNING_AS_ERRORS)
    add_compile_options(-Werror)
  endif()
  # Sanitizers em Debug (GCC/Clang)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  endif()
endif()

# Adiciona a biblioteca afazer_util
add_library(afazer_util
  src/util/arquivo_lock.cpp
  src/util/tempo.cpp
)
target_include_directories(afazer_util PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Adiciona o executável afazer
add_executable(afazer
  src/cli/main.cpp
  src/cli/app.cpp
)
target_include_directories(afazer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Adiciona os arquivos do domínio
add_library(afazer_dominio
  src/dominio/tarefa.cpp
  src/dominio/filtros.cpp
  src/dominio/exportador.cpp
  src/dominio/relatorios.cpp
)
target_include_directories(afazer_dominio PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Linka a biblioteca afazer_util ao executável afazer
target_link_libraries(afazer PRIVATE afazer_util)

# Linka a biblioteca afazer_dominio ao executável afazer
target_link_libraries(afazer PRIVATE afazer_dominio)

# Define a versão como macro no código
target_compile_definitions(afazer PRIVATE AFAZER_VERSION="${PROJECT_VERSION}")

# Baixa e adiciona o Catch2 como dependência
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0 # Use a versão mais recente estável
)
FetchContent_MakeAvailable(Catch2)

# Baixa e adiciona a biblioteca JSON
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2 # Use a versão mais recente estável
)
FetchContent_MakeAvailable(nlohmann_json)

# Linka a biblioteca JSON ao domínio
target_link_libraries(afazer_dominio PUBLIC nlohmann_json::nlohmann_json)

# Adiciona o executável de testes
add_executable(tests
  tests/tarefas_tests.cpp
  tests/filtros_tests.cpp
)
target_link_libraries(tests PRIVATE afazer_dominio Catch2::Catch2WithMain)

# Instalação básica
install(TARGETS afazer RUNTIME DESTINATION bin)
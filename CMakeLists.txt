cmake_minimum_required(VERSION 3.20)

# Nome e versão do projeto
project(aFazer LANGUAGES CXX VERSION 0.1.0)

# Opções (usaremos mais nos próximos capítulos)
option(AFAZER_BUILD_TESTES "Construir testes" ON)
option(AFAZER_BUILD_TUI "Construir TUI (ncurses/PDCurses)" OFF)
option(AFAZER_WARNING_AS_ERRORS "Tratar warnings como erros" OFF)

# Padrão de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Exporta compile_commands.json (bom para clangd/linters)
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

# Executável inicial (somente CLI por enquanto)
add_executable(afazer
  src/cli/main.cpp
  src/cli/app.cpp
)

target_include_directories(afazer PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Define a versão como macro no código
target_compile_definitions(afazer PRIVATE AFAZER_VERSION="${PROJECT_VERSION}")

# Instalação básica (veremos mais adiante)
install(TARGETS afazer RUNTIME DESTINATION bin)
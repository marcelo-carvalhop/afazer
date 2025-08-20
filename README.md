# aFazer 

**aFazer** é um gerenciador de tarefas em linha de comando (CLI) escrito em C++ moderno. 
O projeto é desenvolvido com foco em boas práticas, modularidade e portabilidade (Linux e Windows)

## Funcionalidades

- Exibe ajuda e versão do programa: 
    - `afazer --ajuda`
    - `afazer --versao`

## Como compilar 

### Pré-requisitos
    **C++ 20**: GCC 10+, Clang 12+, ou MSCV 2022.
    **CMake**: Versão 3.20 ou superior. 
    **Ninja**: (opcional): Para builds mais rápidos.

### Instruções 

1. Clone o repositório:

```bash
   git clone https://github.com/marcelo-carvalhop/afazer.git
   cd afazer
```
2. Configure o build (Debug):

```bash
cmake --preset debug
```

3. Compile:

```bash
cmake --build --preset debug
```
4. Execute:

```bash
./build/afazer --ajuda
```

## Estrutura do Projeto

- src/: Código-fonte do projeto.
- include/: Headers públicos.
- build/: diretório de build (gerado pelo CMake).
- CMakePresets.json: Configurações de build reutilizáveis.

## Licença 

Este projeto está sob a Licença Apache 2.0. Consulte o arquivo LICENSE par amais detalhes.
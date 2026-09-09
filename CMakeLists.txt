cmake_minimum_required(VERSION 3.10)

# Nome do projeto e a linguagem utilizada
project(Locar C)

# Define o padrão C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Inclui os diretórios onde os arquivos de cabeçalho (.h) estão localizados
include_directories(include)

# Lista de arquivos fonte (.c) baseada na estrutura do projeto
set(SOURCES
    main.c
    src/clientes.c
    src/locacoes.c
    src/relatorios.c
    src/veiculos.c
    src/vendas.c
    src/util.c
    data/dados.c
    src/manutencao.c
)

# Cria o executável chamando-o de "Locar"
add_executable(Locar ${SOURCES})

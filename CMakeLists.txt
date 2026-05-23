cmake_minimum_required(VERSION 3.28) # Or your current version
project(code_cat_coffee C)

set(CMAKE_C_STANDARD 11)

# Add both main.c and sqlite3.c to the executable
add_executable(main programa-c/main.c programa-c/sqlite3.c
        programa-c/bd.c
        programa-c/categoria_produto.c
        programa-c/cliente.c
        programa-c/forma_pagamento.c
        programa-c/funcionario.c
        programa-c/pedido.c
        programa-c/produto.c
        programa-c/menu.c
        programa-c/utilidades.c)

# Link the threading library (required for SQLite)
find_package(Threads REQUIRED)
target_link_libraries(main PRIVATE Threads::Threads)
/**
 *              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026103546 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Biblioteca Code Cat Coffee - Módulo Clientes
 */

#include <stdio.h>
#include "bd.h"
#include "cliente.h"
#include "utilidades.h"

/*
 * Aqui criamos variáveis em comum para uso na biblioteca
 */
int cli_retorno; // Para armazenar o cli_retorno das funções chamadas do SQLite
sqlite3_stmt* cli_statement; // Para armazenar prepared cli_statements

/**
 * Roda uma consulta de busca de clientes e imprime o resultado da busca.
 */
void cliente_buscar()
{
    char nome[255];

    printf("Digite o nome do cliente a buscar: ");
    entrada_string(nome, sizeof(nome));

    cli_retorno = bd_prepara_consulta("SELECT id_cliente, nome FROM cliente WHERE nome LIKE '%?%' ORDER BY nome ASC;");

    if (cli_retorno != 0)
    {
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(cli_statement, 1, nome, -1, SQLITE_STATIC);

    printf("Clientes encontrados:\n");

    bd_imprimir_resultados_tabela(cli_statement);

    // Limpeza pós-execução
    sqlite3_finalize(cli_statement);
}

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 */
void cliente_listar()
{
    cli_retorno = bd_prepara_consulta("SELECT id_cliente, nome FROM cliente ORDER BY nome ASC;");

    if (cli_retorno != 0)
    {
        return;
    }

    printf("Clientes cadastrados:\n");

    bd_imprimir_resultados_tabela(cli_statement);

    // Limpeza pós-execução
    sqlite3_finalize(cli_statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar clientes e pede para o usuário selecionar um por ID.
 * @return ID do cliente selecionado.
 */
int cliente_selecionar()
{
    int cliente = 0, opcao;

    while (cliente == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um cliente\n  2) listar todos\n");
        opcao = entrada_int();

        if (opcao == 1)
        {
            cliente_buscar();
        }
        else if (opcao == 2)
        {
            cliente_listar();
        }
        else
        {
            printf("Opção inválida!\n");
            continue;
        }

        printf("Qual o ID do cliente você deseja selecionar?\n");
        cliente = entrada_int();
    }

    return cliente;
}

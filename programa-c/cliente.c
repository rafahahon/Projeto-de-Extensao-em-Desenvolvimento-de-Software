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

/**
 * Roda uma consulta de busca de clientes e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void cliente_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do cliente a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_cliente, nome FROM cliente WHERE nome LIKE ? ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    // Coloca % para poder pegar parciais
    sprintf(termo_busca, "%%%s%%", nome);
    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, termo_busca, -1, SQLITE_STATIC);

    printf("Clientes encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void cliente_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_cliente, nome FROM cliente ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Clientes cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar clientes e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do cliente selecionado.
 */
int cliente_selecionar(sqlite3* bd)
{
    int cliente = 0, opcao;

    while (cliente == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um cliente\n  2) listar todos\n");
        opcao = entrada_int();

        if (opcao == 1)
        {
            cliente_buscar(bd);
        }
        else if (opcao == 2)
        {
            cliente_listar(bd);
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

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
 * Biblioteca Code Cat Coffee - Módulo Formas de Pagamento
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "forma_pagamento.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de forma de pagamento e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome da forma de pagamento a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_forma_pagto, nome FROM forma_pagamento WHERE nome LIKE ? ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    // Coloca % para poder pegar parciais
    sprintf(termo_busca, "%%%s%%", nome);
    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, termo_busca, -1, SQLITE_TRANSIENT);

    printf("Formas de Pagamento encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra uma nova forma de pagamento.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da forma de pagamento se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 forma_pagto_cadastrar(sqlite3* bd)
{
    char nome[255];
    int retorno;
    sqlite3_int64 id_forma_pagto;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome da forma de pagamento:\n");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) == 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO forma_pagamento(nome) VALUES (?)",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar forma de pagamento: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar forma de pagamento: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID da forma de pagamento, para referência
    id_forma_pagto = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Forma de pagamento cadastrada!\n");

    return id_forma_pagto;
}

/**
 * Edita uma forma de pagamento existente.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_editar(sqlite3* bd)
{
    char nome[255];
    int retorno;
    sqlite3_int64 id_forma_pagto = 0;
    sqlite3_stmt* statement = NULL;

    id_forma_pagto = forma_pagto_selecionar(bd);

    if (id_forma_pagto == 0)
    {
        printf("Nenhuma forma de pagamento selecionada! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome da forma de pagamento ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    if (strlen(nome) == 0)
    {
        printf("Nome não fornecido, cancelando edição.\n");
        return;
    }

    while (valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    retorno = bd_prepara_consulta(
        bd,
        "UPDATE forma_pagamento SET nome = ? WHERE id_forma_pagto = ?;",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao atualizar forma de pagamento: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(statement, 2, id_forma_pagto);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar forma de pagamento: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Forma de pagamento atualizada com sucesso!\n");
}

/**
 * Roda uma consulta de todas as formas de pagamento e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_forma_pagto, nome FROM forma_pagamento ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Formas de Pagamento cadastradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar formas de pagamento e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da forma de pagamento selecionada.
 */
int forma_pagto_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_forma_pagto = 0, opcao;
    sqlite3_int64 id_nova_forma_pagto = 0;

    while (id_forma_pagto == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar uma forma de pagamento\n  2) listar todas\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            forma_pagto_buscar(bd);
            break;
        case 2:
            forma_pagto_listar(bd);
            break;
        case 3:
            id_nova_forma_pagto = forma_pagto_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_nova_forma_pagto > 0)
        {
            printf("Deseja selecionar esta forma de pagamento criada? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_nova_forma_pagto;
            }
        }

        printf("Qual o ID da forma de pagamento você deseja selecionar?\n");
        id_forma_pagto = entrada_int();
    }

    return id_forma_pagto;
}

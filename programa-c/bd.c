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
 * Biblioteca Code Cat Coffee - Banco de Dados
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "utilidades.h"

/*
 * Aqui criamos variáveis em comum para uso na biblioteca
 */
int bd_retorno; // Para armazenar o retorno das funções chamadas do SQLite
char* bd_erro; // Para armazenar o último erro do BD
sqlite3_stmt* bd_statement; // Para armazenar prepared statements
sqlite3* bd; // Referência ao banco de dados como "bd" para facilitar na hora de chamar

/**
 * Conecta com o banco de dados SQLite para uso durante a execução.
 */
void bd_conectar()
{
    // Aqui nós iniciamos a conexão com o banco de dados
    bd_retorno = sqlite3_open_v2(
        "file://./../data/codecatcoffee.sqlite",
        &bd,
        SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_PRIVATECACHE,
        "win32"
    );

    // Aqui nós vamos testar se a conexão com o banco de dados deu certo, e caso não tenha dado,
    // vai mostrar a mensagem "Erro na conexão"
    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro na conexão ao banco de dados: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        exit(1);
    }

    // Caso a conexão tenha dado certo, essa será a mensagem exibida
    printf("Conectado ao banco!\n");

    bd_verificar_integridade();
}

/**
 * Inicializa o Banco de Dados com a estrutura básica de tabelas para o programa.
 */
void bd_criar_tabelas()
{
    printf("Criando tabelas...\n");
    // Vamos ler o arquivo SQL num buffer de string
    char* query = ler_arquivo("file://./../banco-de-dados/esquema-sqlite.sql");

    // Executa a query
    bd_retorno = sqlite3_exec(bd, query, NULL, 0, &bd_erro);

    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro ao inicializar o banco de dados: %s\n", bd_erro);
        sqlite3_free(bd_erro);
        sqlite3_close(bd);
        exit(1);
    }

    printf("Banco de dados inicializado com sucesso!\n");
}

/**
 * Roda uma consulta select e imprime os nomes das colunas e cada linha no resultado.
 * @param stmt A consulta preparada (prepared statement)
 */
void bd_imprimir_resultados_tabela(sqlite3_stmt* stmt)
{
    int i,
        total_col = sqlite3_column_count(stmt),
        rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE)
    {
        printf("Nenhum resultado encontrado.\n");
        return;
    }

    // 1. Mostra o cabeçalho
    printf("\n");

    for (i = 0; i < total_col; i++)
    {
        // %-15s é: string, alinhada a esquerda (COMUNISTA!!), 15 caracteres de largura
        printf("%-15s | ", sqlite3_column_name(stmt, i));
    }

    printf("\n");

    // 2. Separador de linhas/colunas
    for (i = 0; i < total_col; i++)
    {
        printf("----------------|");
    }

    printf("\n");

    // 3. Mostra as linhas
    while (rc == SQLITE_ROW)
    {
        for (i = 0; i < total_col; i++)
        {
            const char* val = (const char*)sqlite3_column_text(stmt, i);
            printf("%-15s | ", val ? val : "NULL");
        }

        printf("\n");
        rc = sqlite3_step(stmt);
    }

    printf("\n");
}

/**
 * Limpa e fecha tudo relativo a Banco de Dados antes de finalizar
 */
void bd_limpa_fecha()
{
    sqlite3_finalize(bd_statement);
    sqlite3_free(bd_erro);
    sqlite3_close(bd);
}

/**
 * Preenche as tabelas do Banco de Dados com dados de exemplo.
 */
void bd_popular_tabelas()
{
    printf("Populando tabelas...\n");
    // Vamos ler o arquivo SQL num buffer de string
    char* query = ler_arquivo("file://./../banco-de-dados/esquema-sql-insercao.sql");

    // Executa a query
    bd_retorno = sqlite3_exec(bd, query, NULL, 0, &bd_erro);

    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro ao popular o banco de dados: %s\n", bd_erro);
        sqlite3_free(bd_erro);
        return;
    }

    printf("Banco de dados populado com sucesso!\n");
}

/**
 * Cria uma prepared statement para a consulta SQL passada.
 * @param query A consulta SQL para ser preparada.
 * @return 0 se sucesso ao preparar a consulta, caso contrário, 1
 */
int bd_prepara_consulta(char* query)
{
    bd_retorno = sqlite3_prepare_v3(
        bd, /* Referência do banco de dados */
        query, /* Consulta SQL, UTF-8 encoded */
        -1, /* Tamanho máximo da consulta em bytes. */
        SQLITE_PREPARE_FROM_DDL | SQLITE_PREPARE_NORMALIZE, /* Zero ou mais flags SQLITE_PREPARE_ */
        &bd_statement, /* SAIDA: Referencia a statement */
        NULL /* SAIDA: Ponteiro para a parte nao usada da query */
    );

    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro ao preparar a consulta: %s\n", sqlite3_errmsg(bd));
        return 1;
    }

    return 0;
}

/**
 * Verifica a integridade do Banco de Dados através das tabelas presentes.
 */
void bd_verificar_integridade()
{
    char *query,
         confirmacao,
         *tabelasEsperadas[12] = {
             "cliente", "funcionario", "mesa", "reserva_mesa", "playground", "sessoes_playground", "gato",
             "produto", "categoria_produto", "pedido", "itens_pedido", "forma_pagamento"
         };
    int tabelasRetornadas[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    query = "SELECT name FROM sqlite_schema WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
    bd_retorno = sqlite3_prepare_v2(bd, query, -1, &bd_statement, 0);

    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro ao verificar integridade do banco de dados: %s\n", bd_erro);
        sqlite3_free(bd_erro);
        sqlite3_close(bd);
        exit(1);
    }

    // Vamos agora verificar linha a linha dos resultados e popular se todas as tabelas esperadas foram retornadas
    while (sqlite3_step(bd_statement) == SQLITE_ROW)
    {
        // Coluna 0 é o 'name' da nossa consulta SELECT
        const char* nomeTabela = (const char*)sqlite3_column_text(bd_statement, 0);

        if (nomeTabela == NULL) continue; // Verificação de segurança

        for (int i = 0; i < 12; i++)
        {
            // strcmp retorna 0 se as strings forem idênticas
            if (strcmp(tabelasEsperadas[i], nomeTabela) == 0)
            {
                tabelasRetornadas[i] = 1;
                break;
            }
        }
    }

    sqlite3_finalize(bd_statement);

    for (int i = 0; i < 12; i++)
    {
        if (tabelasRetornadas[i] == 0)
        {
            printf("Tabela %s não está presente no banco de dados. ", tabelasEsperadas[i]);
            printf("Deseja inicializar as tabelas no banco de dados? [s/n]\n");
            scanf("%s", &confirmacao);

            if (confirmacao == 's')
            {
                bd_criar_tabelas();

                printf("Deseja popular as tabelas com dados de exemplo? [s/n]\n");
                scanf("%s", &confirmacao);

                if (confirmacao == 's')
                {
                    bd_popular_tabelas();
                }

                break;
            }
        }
    }
}

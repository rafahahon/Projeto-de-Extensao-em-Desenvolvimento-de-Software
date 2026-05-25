/**
 *              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
* ================================================
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026101346 |
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
#include "sqlite3.h"
#include "utilidades.h"

/**
 * Conecta com o banco de dados SQLite para uso durante a execução.
 * @return A conexão do banco de dados criada.
 */
sqlite3* bd_conectar()
{
    sqlite3* bd = NULL;
    // Aqui nós iniciamos a conexão com o banco de dados
    const int bd_retorno = sqlite3_open_v2(
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

    bd_verificar_integridade(bd);

    return bd;
}

/**
 * Inicializa o Banco de Dados com a estrutura básica de tabelas para o programa.
 * @param bd A referência à conexão do banco de dados.
 */
void bd_criar_tabelas(sqlite3* bd)
{
    printf("Criando tabelas...\n");
    const char* query = ler_arquivo("file://./../banco-de-dados/esquema-sqlite.sql");
    // Vamos ler o arquivo SQL num buffer de string
    char* bd_erro;

    // Executa a query
    const int bd_retorno = sqlite3_exec(bd, query, NULL, 0, &bd_erro);

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
 * Imprime um texto e preenche com espaços até atingir a largura visual desejada.
 * @param texto O texto a ser impresso
 * @param largura_desejada A largura da coluna desejada
 */
void bd_imprimir_celula_alinhada(const char* texto, const int largura_desejada)
{
    const char* texto_final = texto ? texto : "NULL";

    // Imprime o texto normalmente
    printf("%s", texto_final);

    // Calcula quantos espaços faltam baseados no tamanho visual (e não em bytes)
    const int espacos_faltando = largura_desejada - tamanho_visual_utf8(texto_final);

    // Imprime os espaços
    for (int i = 0; i < espacos_faltando; i++)
    {
        printf(" ");
    }
}

/**
 * Roda uma consulta select e imprime os nomes das colunas e cada linha no resultado.
 * @param bd_statement A consulta preparada (prepared statement)
 */
void bd_imprimir_resultados_tabela(sqlite3_stmt* bd_statement)
{
    const int total_col = sqlite3_column_count(bd_statement);
    int i,
        resultado = sqlite3_step(bd_statement);
    int* larguras;

    if (resultado == SQLITE_DONE)
    {
        printf("Nenhum resultado encontrado.\n");
        return;
    }

    // Alocamos um array para guardar o tamanho máximo de cada coluna
    larguras = (int*)malloc(total_col * sizeof(int));

    // Iniciamos as larguras baseadas no nome do cabeçalho da coluna
    for (i = 0; i < total_col; i++)
    {
        larguras[i] = tamanho_visual_utf8(sqlite3_column_name(bd_statement, i));
    }

    // Varremos os dados para ver se alguma linha tem um texto maior que o cabeçalho
    do
    {
        for (i = 0; i < total_col; i++)
        {
            const char* valor = (const char*)sqlite3_column_text(bd_statement, i);
            const int tam_valor = valor ? tamanho_visual_utf8(valor) : 4; // 4 é o tamanho da palavra "NULL"

            if (tam_valor > larguras[i])
            {
                larguras[i] = tam_valor;
            }
        }

        resultado = sqlite3_step(bd_statement);
    }
    while (resultado == SQLITE_ROW);

    // Volta a consulta para o estado inicial para lê-la novamente
    sqlite3_reset(bd_statement);

    // Mostra o cabeçalho
    printf("\n");

    for (i = 0; i < total_col; i++)
    {
        bd_imprimir_celula_alinhada(sqlite3_column_name(bd_statement, i), larguras[i]);
        printf(" | ");
    }

    printf("\n");

    // Separador (Gera os tracinhos)
    for (i = 0; i < total_col; i++)
    {
        for (int j = 0; j < larguras[i]; j++)
        {
            printf("-");
        }

        printf("-| ");
    }

    printf("\n");

    // Linhas de dados
    while (sqlite3_step(bd_statement) == SQLITE_ROW)
    {
        for (i = 0; i < total_col; i++)
        {
            const char* val = (const char*)sqlite3_column_text(bd_statement, i);
            bd_imprimir_celula_alinhada(val, larguras[i]);
            printf(" | ");
        }

        printf("\n");
    }

    printf("\n");

    // Limpeza de memória do array que alocamos
    free(larguras);
}

/**
 * Preenche as tabelas do Banco de Dados com dados de exemplo.
 * @param bd A referência à conexão do banco de dados.
 */
void bd_popular_tabelas(sqlite3* bd)
{
    printf("Populando tabelas...\n");
    const char* query = ler_arquivo("file://./../banco-de-dados/esquema-sql-insercao.sql");
    // Vamos ler o arquivo SQL num buffer de string
    char* bd_erro;

    // Executa a query
    const int bd_retorno = sqlite3_exec(bd, query, NULL, 0, &bd_erro);

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
 * @param bd A referência à conexão do banco de dados.
 * @param query A consulta SQL para ser preparada.
 * @param bd_statement A consulta preparada a ser estruturada.
 * @return 0 se sucesso ao preparar a consulta, caso contrário, 1
 */
int bd_prepara_consulta(sqlite3* bd, const char* query, sqlite3_stmt** bd_statement)
{
    const int bd_retorno = sqlite3_prepare_v3(
        bd, /* Referência do banco de dados */
        query, /* Consulta SQL, UTF-8 encoded */
        -1, /* Tamanho máximo da consulta em bytes. */
        SQLITE_PREPARE_FROM_DDL | SQLITE_PREPARE_NORMALIZE, /* Zero ou mais flags SQLITE_PREPARE_ */
        bd_statement, /* SAIDA: Referencia a statement */
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
 * @param bd A referência à conexão do banco de dados.
 */
void bd_verificar_integridade(sqlite3* bd)
{
    char *query,
         confirmacao[5],
         *tabelasEsperadas[12] = {
             "cliente", "funcionario", "mesa", "reserva_mesa", "playground", "sessoes_playground", "gato",
             "produto", "categoria_produto", "pedido", "itens_pedido", "forma_pagamento"
         };
    int bd_retorno, tabelasRetornadas[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    sqlite3_stmt* bd_statement;

    query = "SELECT name FROM sqlite_schema WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
    bd_retorno = sqlite3_prepare_v2(bd, query, -1, &bd_statement, 0);

    if (bd_retorno != SQLITE_OK)
    {
        printf("Erro ao verificar integridade do banco de dados: %s\n", sqlite3_errmsg(bd));
        sqlite3_free(NULL);
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
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                bd_criar_tabelas(bd);

                printf("Deseja popular as tabelas com dados de exemplo? [s/n]\n");
                entrada_string(confirmacao, sizeof(confirmacao));

                if (strcmp(confirmacao, "s") == 0)
                {
                    bd_popular_tabelas(bd);
                }

                break;
            }
        }
    }
}

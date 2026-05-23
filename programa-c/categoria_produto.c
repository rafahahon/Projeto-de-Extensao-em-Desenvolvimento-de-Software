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
 * Biblioteca Code Cat Coffee - Módulo Categorias de Produto
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "categoria_produto.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de categoria de produto e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void cat_prod_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome da categoria a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_cat_produto, nome FROM categoria_produto WHERE nome LIKE ? ORDER BY nome ASC;",
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

    printf("Categorias encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra uma nova categoria de produto.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da categoria de produto se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 cat_prod_cadastrar(sqlite3* bd)
{
    char nome[255];
    int retorno;
    sqlite3_int64 id_cat_produto;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome da categoria de produto:\n");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) == 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO categoria_produto(nome) VALUES (?)",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar categoria de produto: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar categoria de produto: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID da categoria de produto, para referência
    id_cat_produto = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Categoria de Produto cadastrada!\n");

    return id_cat_produto;
}

/**
 * Edita uma categoria de produto existente.
 * @param bd A referência à conexão do banco de dados.
 */
void cat_prod_editar(sqlite3* bd)
{
    char nome[255];
    int retorno;
    sqlite3_int64 id_cat_produto = 0;
    sqlite3_stmt* statement = NULL;

    id_cat_produto = cat_prod_selecionar(bd);

    if (id_cat_produto == 0)
    {
        printf("Nenhuma categoria de produto selecionada! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome da categoria ou enter para pular:\n");
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
        "UPDATE categoria_produto SET nome = ? WHERE id_cat_produto = ?;",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao atualizar categoria de produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(statement, 2, id_cat_produto);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar categoria de produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Categoria de produto atualizada com sucesso!\n");
}

/**
 * Roda uma consulta de todas as categorias de produto e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void cat_prod_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_cat_produto, nome FROM categoria_produto ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Categorias cadastradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar categorias de produto e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da categoria de produto selecionada.
 */
int cat_prod_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_cat_produto = 0, opcao;
    sqlite3_int64 id_novo_cat_produto = 0;

    while (id_cat_produto == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar uma categoria\n  2) listar todas\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            cat_prod_buscar(bd);
            break;
        case 2:
            cat_prod_listar(bd);
            break;
        case 3:
            id_novo_cat_produto = cat_prod_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_cat_produto > 0)
        {
            printf("Deseja selecionar esta categoria criada? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_cat_produto;
            }
        }

        printf("Qual o ID da categoria você deseja selecionar?\n");
        id_cat_produto = entrada_int();
    }

    return id_cat_produto;
}

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
 * Biblioteca Code Cat Coffee - Módulo Categoria de Produtos
 */

#include <stdio.h>
#include "bd.h"
#include "categoria_produto.h"
#include "utilidades.h"

/*
 * Aqui criamos variáveis em comum para uso na biblioteca
 */
int cat_prod_retorno; // Para armazenar o cat_prod_retorno das funções chamadas do SQLite
sqlite3_stmt* cat_prod_statement; // Para armazenar prepared cat_prod_statements

/**
 * Roda uma consulta de busca de categoria de produto e imprime o resultado da busca.
 */
void cat_prod_buscar()
{
    char nome[255];

    printf("Digite o nome da categoria a buscar: ");
    entrada_string(nome, sizeof(nome));

    cat_prod_retorno = bd_prepara_consulta(
        "SELECT id_cat_produto, nome FROM categoria_produto WHERE nome LIKE '%?%' ORDER BY nome ASC;");

    if (cat_prod_retorno != 0)
    {
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(cat_prod_statement, 1, nome, -1, SQLITE_STATIC);

    printf("Categorias encontradas:\n");

    bd_imprimir_resultados_tabela(cat_prod_statement);

    // Limpeza pós-execução
    sqlite3_finalize(cat_prod_statement);
}

/**
 * Roda uma consulta de todas as categorias de produto e lista os resultados.
 */
void cat_prod_listar()
{
    cat_prod_retorno = bd_prepara_consulta("SELECT id_cat_produto, nome FROM categoria_produto ORDER BY nome ASC;");

    if (cat_prod_retorno != 0)
    {
        return;
    }

    printf("Categorias cadastradas:\n");

    bd_imprimir_resultados_tabela(cat_prod_statement);

    // Limpeza pós-execução
    sqlite3_finalize(cat_prod_statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar categorias de produto e pede para o usuário selecionar um por ID.
 * @return ID da categoria de produto selecionada.
 */
int cat_prod_selecionar()
{
    int id_cat_produto = 0, opcao;

    while (id_cat_produto == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar uma categoria\n  2) listar todas\n");
        opcao = entrada_int();

        if (opcao == 1)
        {
            cat_prod_buscar();
        }
        else if (opcao == 2)
        {
            cat_prod_listar();
        }
        else
        {
            printf("Opção inválida!\n");
            continue;
        }

        printf("Qual o ID da categoria você deseja selecionar?\n");
        id_cat_produto = entrada_int();
    }

    return id_cat_produto;
}

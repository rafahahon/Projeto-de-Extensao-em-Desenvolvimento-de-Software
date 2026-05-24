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
 * Biblioteca Code Cat Coffee - Módulo Produtos
 */

#include <stdio.h>
#include "bd.h"
#include "categoria_produto.h"
#include "produto.h"
#include "utilidades.h"
/*
 * Aqui criamos variáveis em comum para uso na biblioteca
 */
int prod_retorno; // Para armazenar o prod_retorno das funções chamadas do SQLite
sqlite3_stmt* prod_statement; // Para armazenar prepared prod_statements
sqlite3* prod_bd; // Referência ao banco de dados como "prod_bd" para facilitar na hora de chamar

/**
 * Roda uma consulta de busca de produtos disponíveis em estoque e imprime o resultado da busca.
 */
void produto_buscar()
{
    char nome[255];

    printf("Digite o nome produto a buscar: ");
    entrada_string(nome, sizeof(nome));

    prod_retorno = bd_prepara_consulta(
        "SELECT id_produto, nome, preco, quantidade_estoque FROM produto WHERE nome LIKE '%?%' AND quantidade_estoque > 0 ORDER BY nome ASC;");

    if (prod_retorno != 0)
    {
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(prod_statement, 1, nome, -1, SQLITE_STATIC);

    printf("Produtos encontrados:\n");

    bd_imprimir_resultados_tabela(prod_statement);

    // Limpeza pós-execução
    sqlite3_finalize(prod_statement);
}

/**
 * Cadastra um novo produto no banco de dados.
 */
void produto_cadastrar()
{
    char nome[255];
    float preco;
    int quantidade, categoria;

    // aqui nos pedimos para o usuário o nome do produto
    printf("Nome do produto: ");
    entrada_string(nome, sizeof(nome));

    // aqui pedimos o preço
    printf("Preço: ");
    preco = entrada_float();

    // aqui pedimos a quantidade
    printf("Quantidade: ");
    quantidade = entrada_int();

    // aqui pedimos a categoria
    categoria = cat_prod_selecionar();

    prod_retorno = bd_prepara_consulta("INSERT INTO produto(nome, preco, quantidade, categoria) VALUES (?, ?, ?, ?)");

    if (prod_retorno != 0)
    {
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(prod_statement, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_double(prod_statement, 2, preco);
    sqlite3_bind_int(prod_statement, 3, quantidade);
    sqlite3_bind_int(prod_statement, 4, categoria);

    // Rodamos a consulta
    prod_retorno = sqlite3_step(prod_statement);

    if (prod_retorno != SQLITE_DONE)
    {
        printf("prod_erro ao cadastrar produto: %s\n", sqlite3_errmsg(prod_bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(prod_statement);

    printf("Produto cadastrado!\n");
}

/**
 * Roda uma consulta de todos os produtos disponíveis em estoque e lista os resultados.
 */
void produto_listar()
{
    prod_retorno = bd_prepara_consulta(
        "SELECT id_produto, nome, preco, quantidade_estoque FROM produto WHERE quantidade_estoque > 0 ORDER BY nome ASC;");

    if (prod_retorno != 0)
    {
        return;
    }

    printf("Produtos cadastrados:\n");

    bd_imprimir_resultados_tabela(prod_statement);

    // Limpeza pós-execução
    sqlite3_finalize(prod_statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar produtos disponíveis em estoque e pede para o usuário
 * selecionar um por ID.
 * @return ID, nome, preço e quantidade em estoque do produto selecionado.
 */
int produto_selecionar()
{
    int produto = 0, opcao;

    while (produto == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um produto\n  2) listar todos\n");
        opcao = entrada_int();

        if (opcao == 1)
        {
            produto_buscar();
        }
        else if (opcao == 2)
        {
            produto_listar();
        }
        else
        {
            printf("Opção inválida!\n");
            continue;
        }

        printf("Qual o ID do produto você deseja selecionar?\n");
        produto = entrada_int();
    }

    return produto;
}

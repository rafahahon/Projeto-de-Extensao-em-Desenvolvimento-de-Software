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

/**
 * Roda uma consulta de busca de produtos disponíveis em estoque e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void produto_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    int retorno;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome produto a buscar: ");
    entrada_string(nome, sizeof(nome));

    retorno = bd_prepara_consulta(
        bd,
        "SELECT id_produto, nome, preco, quantidade_estoque FROM produto WHERE nome LIKE ? AND quantidade_estoque > 0 ORDER BY nome ASC;",
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

    printf("Produtos encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo produto no banco de dados.
 * @param bd A referência à conexão do banco de dados.
 */
void produto_cadastrar(sqlite3* bd)
{
    char nome[255];
    float preco;
    int quantidade, categoria, retorno;
    sqlite3_stmt* statement = NULL;

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
    categoria = cat_prod_selecionar(bd);

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO produto(nome, preco, quantidade_estoque, id_cat_produto) VALUES (?, ?, ?, ?)",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_double(statement, 2, preco);
    sqlite3_bind_int(statement, 3, quantidade);
    sqlite3_bind_int(statement, 4, categoria);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("prod_erro ao cadastrar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Produto cadastrado!\n");
}

/**
 * Roda uma consulta de todos os produtos disponíveis em estoque e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void produto_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_produto, nome, preco, quantidade_estoque FROM produto WHERE quantidade_estoque > 0 ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Produtos cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar produtos disponíveis em estoque e pede para o usuário
 * selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID, nome, preço e quantidade em estoque do produto selecionado.
 */
int produto_selecionar(sqlite3* bd)
{
    int produto = 0, opcao;

    while (produto == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um produto\n  2) listar todos\n");
        opcao = entrada_int();

        if (opcao == 1)
        {
            produto_buscar(bd);
        }
        else if (opcao == 2)
        {
            produto_listar(bd);
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

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
#include <string.h>
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
    sqlite3_bind_text(statement, 1, termo_busca, -1, SQLITE_TRANSIENT);

    printf("Produtos encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo produto no banco de dados.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do cliente se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 produto_cadastrar(sqlite3* bd)
{
    char nome[255];
    float preco;
    int quantidade, categoria, retorno;
    sqlite3_int64 id_produto;
    sqlite3_stmt* statement = NULL;

    // aqui nos pedimos para o usuário o nome do produto
    printf("Nome do produto: ");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    // aqui pedimos o preço
    printf("Preço unitário: ");
    preco = entrada_float();

    while (preco <= 0)
    {
        printf("Preço precisa ser maior que zero, digite um preço válido:\n");
        preco = entrada_float();
    }

    // aqui pedimos a quantidade
    printf("Quantidade em estoque: ");
    quantidade = entrada_int();

    while (quantidade <= 0)
    {
        printf("Quantidade precisa ser maior que zero, digite uma quantidade válida:\n");
        quantidade = entrada_int();
    }

    // aqui pedimos a categoria
    categoria = cat_prod_selecionar(bd);

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO produto(nome, preco, quantidade_estoque, id_cat_produto) VALUES (?, ?, ?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar produto: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(statement, 2, preco);
    sqlite3_bind_int(statement, 3, quantidade);
    sqlite3_bind_int(statement, 4, categoria);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar produto: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do pedido, para referência
    id_produto = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Produto cadastrado!\n");

    return id_produto;
}

/**
 * Edita um produto existente.
 * @param bd A referência à conexão do banco de dados.
 */
void produto_editar(sqlite3* bd)
{
    char nome[255], query[1024];
    float preco;
    int quantidade, categoria, retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_produto = 0;
    sqlite3_stmt* statement = NULL;

    id_produto = produto_selecionar(bd);

    if (id_produto == 0)
    {
        printf("Nenhum produto selecionado! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome do produto ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    while (strlen(nome) > 0 && valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    // aqui pedimos o preço
    printf("Ditige o novo preço unitário ou enter para pular: ");
    preco = entrada_float();

    while (preco != -1.0f && preco <= 0)
    {
        printf("Preço precisa ser maior que zero, digite um preço válido:\n");
        preco = entrada_float();
    }

    // aqui pedimos a quantidade
    printf("Digite a nova quantidade em estoque ou enter para pular: ");
    quantidade = entrada_int();

    while (quantidade < -1)
    {
        printf("Quantidade precisa ser maior ou igual a zero, digite uma quantidade válida:\n");
        quantidade = entrada_int();
    }

    // aqui pedimos a categoria
    categoria = cat_prod_selecionar(bd);

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (strlen(nome) == 0 && preco == -1.0f && quantidade == -1 && categoria == 0)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE produto SET ");
    tem_campo_anterior = 0;

    if (strlen(nome) > 0)
    {
        strcat(query, "nome = ?");
        tem_campo_anterior = 1;
    }

    if (preco >= 0.0f)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "preco = ?");
        tem_campo_anterior = 1;
    }

    if (quantidade > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "quantidade_estoque = ?");
        tem_campo_anterior = 1;
    }

    if (categoria > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "id_cat_produto = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_produto = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (strlen(nome) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, nome, -1, SQLITE_TRANSIENT);
    }

    if (preco >= 0.0f)
    {
        sqlite3_bind_double(statement, indice_bind++, preco);
    }

    if (quantidade >= 0)
    {
        sqlite3_bind_int(statement, indice_bind++, quantidade);
    }

    if (categoria > 0)
    {
        sqlite3_bind_int(statement, indice_bind++, categoria);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_produto);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Produto atualizado com sucesso!\n");
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
    char confirmacao[5];
    int produto = 0, opcao;
    sqlite3_int64 id_novo_produto = 0;

    while (produto == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um produto\n  2) listar todos\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            produto_buscar(bd);
            break;
        case 2:
            produto_listar(bd);
            break;
        case 3:
            id_novo_produto = produto_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_produto > 0)
        {
            printf("Deseja selecionar este produto criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_produto;
            }
        }

        printf("Qual o ID do produto você deseja selecionar?\n");
        produto = entrada_int();
    }

    return produto;
}

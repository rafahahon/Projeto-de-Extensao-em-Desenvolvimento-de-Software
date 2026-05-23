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
 * Biblioteca Code Cat Coffee - Módulo Pedidos
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bd.h"
#include "cliente.h"
#include "forma_pagamento.h"
#include "pedido.h"
#include "produto.h"
#include "utilidades.h"

/**
 * Adiciona um produto ao pedido.
 * @param bd A referência à conexão do banco de dados.
 * @param id_pedido ID do pedido
 */
void pedido_adicionar_item(sqlite3* bd, sqlite3_int64 id_pedido)
{
    char confirmacao[5], *nome_produto;
    int id_produto = 0, quantidade = 0, prod_quantidade_estoque = 0, retorno;
    float preco_produto = 0, total = 0;
    sqlite3_stmt* statement = NULL;

    do
    {
        id_produto = produto_selecionar(bd);
        retorno = bd_prepara_consulta(
            bd,
            "SELECT nome, preco, quantidade_estoque FROM produto WHERE id = ? LIMIT 1;",
            &statement
        );

        if (retorno != 0)
        {
            return;
        }

        // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
        sqlite3_bind_int(statement, 1, id_produto);

        // Rodamos a consulta
        retorno = sqlite3_step(statement);

        if (retorno != SQLITE_DONE)
        {
            printf("Erro ao obter dados do produto: %s\n", sqlite3_errmsg(bd));
            return;
        }

        // Vamos agora pegar os dados do produto retornado do BD
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            // Coluna 0 é o 'nome' da nossa consulta SELECT
            nome_produto = (char*)sqlite3_column_text(statement, 0);
            preco_produto = (float)sqlite3_column_double(statement, 1);
            prod_quantidade_estoque = sqlite3_column_int(statement, 2);
        }

        sqlite3_finalize(statement);

        // Verificação de segurança
        if (nome_produto == NULL || preco_produto == 0 || prod_quantidade_estoque == 0) continue;

        printf("Quantidade: ");
        quantidade = entrada_int();

        if (quantidade > prod_quantidade_estoque)
        {
            printf("A quantidade digitada (%d) excede a quantidade em estoque (%d) do produto %s", quantidade,
                   prod_quantidade_estoque, nome_produto);
            printf("Deseja mudar a quantidade para %d e continuar? [s/n]", prod_quantidade_estoque);
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") != 0)
            {
                return;
            }

            quantidade = prod_quantidade_estoque;
        }

        // Adiciona o item do pedido no BD
        retorno = bd_prepara_consulta(
            bd,
            "INSERT INTO itens_pedido (id_pedido, id_produto, quantidade_pedida, preco_unitario) VALUES (?, ?, ?, ?, ?);",
            &statement
        );

        if (retorno != 0)
        {
            return;
        }

        // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
        sqlite3_bind_int64(statement, 1, id_pedido);
        sqlite3_bind_int(statement, 1, id_produto);
        sqlite3_bind_int(statement, 1, quantidade);
        sqlite3_bind_double(statement, 1, preco_produto);

        // Rodamos a consulta
        retorno = sqlite3_step(statement);

        if (retorno != SQLITE_DONE)
        {
            printf("Erro ao adicionar item ao pedido: %s\n", sqlite3_errmsg(bd));
            return;
        }

        sqlite3_finalize(statement);

        // Atualiza o total do pedido atual
        total = (float)quantidade * preco_produto;
        retorno = bd_prepara_consulta(bd, "UPDATE pedido SET total = total + ? WHERE id_pedido = ?;", &statement);

        if (retorno != 0)
        {
            return;
        }

        // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
        sqlite3_bind_double(statement, 1, total);
        sqlite3_bind_int64(statement, 2, id_pedido);

        // Rodamos a consulta
        retorno = sqlite3_step(statement);

        if (retorno != SQLITE_DONE)
        {
            printf("Erro atualizar total do pedido: %s\n", sqlite3_errmsg(bd));
            return;
        }

        sqlite3_finalize(statement);

        // Atualiza a quantidade em estoque do produto
        retorno = bd_prepara_consulta(
            bd,
            "UPDATE produto SET quantidade_estoque = quantidade_estoque - ? WHERE id_produto = ?;",
            &statement
        );

        if (retorno != 0)
        {
            return;
        }

        // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
        sqlite3_bind_int(statement, 1, quantidade);
        sqlite3_bind_int64(statement, 2, id_produto);

        // Rodamos a consulta
        retorno = sqlite3_step(statement);

        if (retorno != SQLITE_DONE)
        {
            printf("Erro atualizar quantidade em estoque do produto: %s\n", sqlite3_errmsg(bd));
            return;
        }

        sqlite3_finalize(statement);

        printf("Produto %s adicionado ao pedido %lld! Deseja adicionar outro produto? [s/n]\n", nome_produto,
               id_pedido);
        entrada_string(confirmacao, sizeof(confirmacao));
    }
    while (strcmp(confirmacao, "s") == 0);
}

/**
 * Roda uma consulta de busca de pedidos por nome de cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_buscar_por_cliente(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    int retorno;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do cliente para buscar o pedido: ");
    entrada_string(nome, sizeof(nome));

    retorno = bd_prepara_consulta(
        bd,
        "SELECT id_pedido, cliente.nome AS cliente, forma_pagamento.nome AS forma_pagto, valor_total_pedido as total, data_pedido FROM pedido JOIN cliente ON cliente.id_cliente = pedido.id_cliente JOIN forma_pagamento ON forma_pagamento.id_forma_pagto = pedido.id_forma_pagto WHERE cliente.nome LIKE ? ORDER BY data_pedido DESC;",
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

    printf("Pedidos encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cria um pedido no Banco de Dados.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do pedido se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 pedido_criar(sqlite3* bd)
{
    char confirmacao[5];
    int cliente, *data, retorno;
    float total = 0;
    struct tm dataHelper = {0};
    sqlite3_int64 id_pedido, id_forma_pagto;
    sqlite3_stmt* statement = NULL;

    cliente = cliente_selecionar(bd);
    id_forma_pagto = forma_pagto_selecionar(bd);

    printf("Data do pedido (DD-MM-AAAA ou vazio para hoje): ");

    entrada_data(data);

    if (data == NULL)
    {
        printf("Erro ao obter data.\n");
        return 0;
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
    dataHelper.tm_mday = data[0];
    dataHelper.tm_mon = data[1] - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = data[2] - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    time_t epoch = mktime(&dataHelper);

    if (epoch == -1)
    {
        printf("Erro ao converter data.\n");
        return 0;
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO pedido(id_cliente, id_func, id_forma_pagto, data_pedido, valor_total_pedido) VALUES (?, ?, ?, ?, ?)",
        &statement
    );

    if (retorno != 0)
    {
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int(statement, 1, cliente);
    sqlite3_bind_int(statement, 2, 1); // TODO: substituir por funcionario logado?
    sqlite3_bind_int64(statement, 3, id_forma_pagto);
    sqlite3_bind_int64(statement, 4, epoch);
    sqlite3_bind_double(statement, 5, total);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao criar o pedido: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do pedido, para referência
    id_pedido = sqlite3_last_insert_rowid(bd);
    sqlite3_finalize(statement);

    printf("Pedido criado com sucesso! Deseja adicionar um item? [s/n]\n");
    entrada_string(confirmacao, sizeof(confirmacao));

    if (strcmp(confirmacao, "s") == 0)
    {
        pedido_adicionar_item(bd, id_pedido);
    }

    return id_pedido;
}

/**
 * Edita um pedido existente.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_editar(sqlite3* bd)
{
    char query[1024];
    int retorno, indice_bind;
    sqlite3_int64 id_pedido, id_forma_pagto;
    sqlite3_stmt* statement = NULL;

    id_pedido = pedido_selecionar(bd);

    if (id_pedido == 0)
    {
        printf("Nenhum pedido selecionado! Cancelando edição.\n");
        return;
    }

    id_forma_pagto = forma_pagto_selecionar(bd);

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (id_forma_pagto == 0)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE pedido SET ");

    if (id_forma_pagto > 0)
    {
        strcat(query, "id_forma_pagto = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_pedido = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar pedido: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    if (id_forma_pagto > 0)
    {
        sqlite3_bind_int64(statement, indice_bind++, id_forma_pagto);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_pedido);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar o pedido: %s\n", sqlite3_errmsg(bd));
        return;
    }

    sqlite3_finalize(statement);

    printf("Pedido atualizado com sucesso!");
}

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_pedido, cliente.nome AS cliente, forma_pagamento.nome AS forma_pagto, valor_total_pedido as total, data_pedido FROM pedido JOIN cliente ON cliente.id_cliente = pedido.id_cliente JOIN forma_pagamento ON forma_pagamento.id_forma_pagto = pedido.id_forma_pagto ORDER BY data_pedido DESC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("\nPedidos cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar pedidos e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do pedido selecionado.
 */
int pedido_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int pedido = 0, opcao;
    sqlite3_int64 id_novo_pedido = 0;

    while (pedido == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar um pedido\n  2) listar todos\n  3) criar pedido\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            pedido_buscar_por_cliente(bd);
            break;
        case 2:
            pedido_listar(bd);
            break;
        case 3:
            id_novo_pedido = pedido_criar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_pedido > 0)
        {
            printf("Deseja selecionar este pedido criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_pedido;
            }
        }

        printf("Qual o ID do pedido você deseja selecionar?\n");
        pedido = entrada_int();
    }

    return pedido;
}

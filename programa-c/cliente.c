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
#include <string.h>
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
    sqlite3_bind_text(statement, 1, termo_busca, -1, SQLITE_TRANSIENT);

    printf("Clientes encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo cliente.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do cliente se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 cliente_cadastrar(sqlite3* bd)
{
    char nome[255], email[255], github[255];
    int retorno;
    sqlite3_int64 id_cliente;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do cliente:\n");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite o email do cliente:\n");
    entrada_string(email, sizeof(email));

    printf("Digite o usuário GitHub do cliente:\n");
    entrada_string(github, sizeof(github));

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO cliente(nome, email, github_user) VALUES (?, ?, ?)",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar cliente: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, github, -1, SQLITE_TRANSIENT);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar cliente: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do pedido, para referência
    id_cliente = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Cliente cadastrado!\n");

    return id_cliente;
}

/**
 * Edita um cliente existente.
 * @param bd A referência à conexão do banco de dados.
 */
void cliente_editar(sqlite3* bd)
{
    char nome[255], email[255], github[255], query[1024];
    int retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_cliente = 0;
    sqlite3_stmt* statement = NULL;

    id_cliente = cliente_selecionar(bd);

    if (id_cliente == 0)
    {
        printf("Nenhum cliente selecionado! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome do cliente ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    while (strlen(nome) > 0 && valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite o novo email do cliente ou enter para pular:\n");
    entrada_string(email, sizeof(email));

    printf("Digite o novo usuário GitHub do cliente ou enter para pular:\n");
    entrada_string(github, sizeof(github));

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (strlen(nome) == 0 && strlen(email) == 0 && strlen(github) == 0)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE cliente SET ");
    tem_campo_anterior = 0;

    if (strlen(nome) > 0)
    {
        strcat(query, "nome = ?");
        tem_campo_anterior = 1;
    }

    if (strlen(email) > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "email = ?");
        tem_campo_anterior = 1;
    }

    if (strlen(github) > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "github_user = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_cliente = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar cliente: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (strlen(nome) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, nome, -1, SQLITE_TRANSIENT);
    }

    if (strlen(email) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, email, -1, SQLITE_TRANSIENT);
    }

    if (strlen(github) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, github, -1, SQLITE_TRANSIENT);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_cliente);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar cliente: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Cliente atualizado com sucesso!\n");
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
    char confirmacao[5];
    int cliente = 0, opcao;
    sqlite3_int64 id_novo_cliente = 0;

    while (cliente == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar um cliente\n  2) listar todos\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            cliente_buscar(bd);
            break;
        case 2:
            cliente_listar(bd);
            break;
        case 3:
            id_novo_cliente = cliente_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_cliente > 0)
        {
            printf("Deseja selecionar este cliente criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_cliente;
            }
        }

        printf("Qual o ID do cliente você deseja selecionar?\n");
        cliente = entrada_int();
    }

    return cliente;
}

/**
*              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Santos Nascimento           | 3026101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026101346 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Biblioteca Code Cat Coffee - Módulo Playgrounds
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "playground.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de playgrounds e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do setor playground a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_playground, nome_setor, capacidade_gatos, capacidade_clientes FROM playground WHERE nome_setor LIKE ? ORDER BY nome_setor ASC;",
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

    printf("Playgrounds encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo playground.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do playground se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 playground_cadastrar(sqlite3* bd)
{
    char nome_setor[255];
    int capacidade_gatos, capacidade_clientes, retorno;
    sqlite3_int64 id_playground;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do setor playground:\n");
    entrada_string(nome_setor, sizeof(nome_setor));

    while (valida_string(nome_setor, 3, 1) <= 0)
    {
        printf("Nome do setor não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome_setor, sizeof(nome_setor));
    }

    printf("Digite a capacidade máxima de gatos do playground:\n");
    capacidade_gatos = entrada_int();

    printf("Digite a capacidade máxima de clientes do playground:\n");
    capacidade_clientes = entrada_int();

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO playground(nome_setor, capacidade_gatos, capacidade_clientes) VALUES (?, ?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar playground: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome_setor, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 2, capacidade_gatos);
    sqlite3_bind_int(statement, 3, capacidade_clientes);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar playground: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do playground, para referência
    id_playground = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Playground cadastrado!\n");

    return id_playground;
}

/**
 * Edita um playground existente.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_editar(sqlite3* bd)
{
    char nome[255], query[1024];
    int capacidade_gatos, capacidade_clientes, retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_playground = 0;
    sqlite3_stmt* statement = NULL;

    id_playground = playground_selecionar(bd);

    if (id_playground == 0)
    {
        printf("Nenhum playground selecionado! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome do setor playground ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    while (strlen(nome) > 0 && valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome do setor não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite a nova capacidade máxima de gatos do playground ou enter para pular:\n");
    capacidade_gatos = entrada_int();

    printf("Digite a nova capacidade máxima de clientes do playground ou enter para pular:\n");
    capacidade_clientes = entrada_int();

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (strlen(nome) == 0 && capacidade_gatos == -1 && capacidade_clientes == -1)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE playground SET ");
    tem_campo_anterior = 0;

    if (strlen(nome) > 0)
    {
        strcat(query, "nome_setor = ?");
        tem_campo_anterior = 1;
    }

    if (capacidade_gatos > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "capacidade_gatos = ?");
        tem_campo_anterior = 1;
    }

    if (capacidade_clientes > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "capacidade_clientes = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_playground = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar playground: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (strlen(nome) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, nome, -1, SQLITE_TRANSIENT);
    }

    if (capacidade_gatos > -1)
    {
        sqlite3_bind_int(statement, indice_bind++, capacidade_gatos);
    }

    if (capacidade_clientes > -1)
    {
        sqlite3_bind_int(statement, indice_bind++, capacidade_clientes);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_playground);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar playground: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Playground atualizado com sucesso!\n");
}

/**
 * Roda uma consulta de todos os playgrounds e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_playground, nome_setor, capacidade_gatos, capacidade_clientes FROM playground ORDER BY nome_setor ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Playgrounds cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar playgrounds e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do playground selecionado.
 */
int playground_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int playground = 0, opcao;
    sqlite3_int64 id_novo_playground = 0;

    while (playground == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar um playground\n  2) listar todos\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            playground_buscar(bd);
            break;
        case 2:
            playground_listar(bd);
            break;
        case 3:
            id_novo_playground = playground_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_playground > 0)
        {
            printf("Deseja selecionar este playground criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_playground;
            }
        }

        printf("Qual o ID do playground você deseja selecionar?\n");
        playground = entrada_int();
    }

    return playground;
}

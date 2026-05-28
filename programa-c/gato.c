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
 * Biblioteca Code Cat Coffee - Módulo Gatos
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "gato.h"

#include "playground.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de gatos e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do gato a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_gato, nome, personalidade, status_gato, playground.nome_setor AS playground FROM gato JOIN playground ON playground.id_playground = gato.id_playground WHERE nome LIKE ? ORDER BY nome ASC;",
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

    printf("Gatos encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo gato.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do gato se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 gato_cadastrar(sqlite3* bd)
{
    char nome[255], personalidade[255];
    int status_gato, retorno;
    sqlite3_int64 id_gato, id_playground;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do gato:\n");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite a personalidade do gato:\n");
    entrada_string(personalidade, sizeof(personalidade));

    printf("Digite o status do gato:\n");
    status_gato = entrada_int();

    id_playground = playground_selecionar(bd);

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO gato(nome, personalidade, status_gato, id_playground) VALUES (?, ?, ?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar gato: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, personalidade, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 3, status_gato);
    sqlite3_bind_int64(statement, 3, id_playground);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar gato: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do gato, para referência
    id_gato = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Gato cadastrado!\n");

    return id_gato;
}

/**
 * Edita um gato existente.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_editar(sqlite3* bd)
{
    char nome[255], personalidade[255], query[1024];
    int status_gato, retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_gato = 0, id_playground;
    sqlite3_stmt* statement = NULL;

    id_gato = gato_selecionar(bd);

    if (id_gato == 0)
    {
        printf("Nenhum gato selecionado! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome do gato ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    while (strlen(nome) > 0 && valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite a nova personalidade do gato ou enter para pular:\n");
    entrada_string(personalidade, sizeof(personalidade));

    printf("Digite o novo status do gato ou enter para pular:\n");
    status_gato = entrada_int();

    id_playground = playground_selecionar(bd);

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (strlen(nome) == 0 && strlen(personalidade) == 0 && status_gato == -1 && id_playground == 0)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE gato SET ");
    tem_campo_anterior = 0;

    if (strlen(nome) > 0)
    {
        strcat(query, "nome = ?");
        tem_campo_anterior = 1;
    }

    if (strlen(personalidade) > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "personalidade = ?");
        tem_campo_anterior = 1;
    }

    if (status_gato > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "status_gato = ?");
        tem_campo_anterior = 1;
    }

    if (id_playground > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "id_playground = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_gato = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar gato: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (strlen(nome) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, nome, -1, SQLITE_TRANSIENT);
    }

    if (strlen(personalidade) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, personalidade, -1, SQLITE_TRANSIENT);
    }

    if (status_gato > -1)
    {
        sqlite3_bind_int(statement, indice_bind++, status_gato);
    }

    if (id_playground > 0)
    {
        sqlite3_bind_int64(statement, indice_bind++, id_playground);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_gato);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar gato: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Gato atualizado com sucesso!\n");
}

/**
 * Roda uma consulta de todos os gatos e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_gato, nome, personalidade, status_gato, playground.nome_setor AS playground FROM gato JOIN playground ON playground.id_playground = gato.id_playground ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Gatos cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar gatos e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do gato selecionado.
 */
int gato_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int gato = 0, opcao;
    sqlite3_int64 id_novo_gato = 0;

    while (gato == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar um gato\n  2) listar todos\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            gato_buscar(bd);
            break;
        case 2:
            gato_listar(bd);
            break;
        case 3:
            id_novo_gato = gato_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_gato > 0)
        {
            printf("Deseja selecionar este gato criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_gato;
            }
        }

        printf("Qual o ID do gato você deseja selecionar?\n");
        gato = entrada_int();
    }

    return gato;
}

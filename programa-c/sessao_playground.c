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
 * Biblioteca Code Cat Coffee - Módulo Sessões de Playground
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "cliente.h"
#include "playground.h"
#include "sessao_playground.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de sessão de playground por cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_buscar_por_cliente(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do cliente para buscar suas sessões de playgroud: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_sessao, cliente.nome AS cliente, playground.nome_setor AS playground, hora_entrada, minutos_permanencia, valor_pago FROM sessoes_playground WHERE cliente.nome LIKE ? ORDER BY id_sessao ASC;",
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

    printf("Sessões de Playground encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Roda uma consulta de busca de sessão de playground por playground e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_buscar_por_playground(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do setor playground para buscar as sessões relacionadas: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_sessao, cliente.nome AS cliente, playground.nome_setor AS playground, hora_entrada, minutos_permanencia, valor_pago FROM sessoes_playground JOIN cliente ON cliente.id_cliente = sessoes_playground.id_cliente JOIN playground ON playground.id_playground = sessoes_playground.id_playground WHERE playground.nome_setor LIKE ? ORDER BY id_sessao ASC;",
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

    printf("Sessões de Playground encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra uma nova mesa.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da sessão de playground se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 sessao_play_cadastrar(sqlite3* bd)
{
    int retorno;
    sqlite3_int64 id_sessao, id_cliente, id_playground;
    sqlite3_stmt* statement = NULL;

    id_cliente = cliente_selecionar(bd);

    id_playground = playground_selecionar(bd);

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO sessoes_playground(id_cliente, id_playground) VALUES (?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int64(statement, 1, id_cliente);
    sqlite3_bind_int64(statement, 2, id_playground);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID da sessão de playground, para referência
    id_sessao = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Sessão de Playground cadastrada!\n");

    return id_sessao;
}

/**
 * Edita uma sessão de playground existente.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_editar(sqlite3* bd)
{
    char query[1024];
    int minutos_permanencia, retorno, tem_campo_anterior, indice_bind;
    float valor_pago;
    sqlite3_int64 id_sessao = 0;
    sqlite3_stmt* statement = NULL;

    id_sessao = sessao_play_selecionar(bd);

    if (id_sessao == 0)
    {
        printf("Nenhuma sessão de playground selecionada! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo número minutos de permanência na sessão de playground ou enter para pular:\n");
    minutos_permanencia = entrada_int();

    while (minutos_permanencia != -1 && minutos_permanencia <= 0)
    {
        printf(
            "Número de minutos de permanência na sessão de playground deve ser maior que zero, digite um número válido:\n");
        minutos_permanencia = entrada_int();
    }

    printf("Digite o novo valor pago da sessão de playground ou enter para pular:\n");
    valor_pago = entrada_float();

    while (valor_pago < -1.0f)
    {
        printf("Valor pago deve ser maior ou igual a zero, digite um número válido:\n");
        valor_pago = entrada_float();
    }


    // Verifica se o usuário simplesmente apertou enter em tudo
    if (minutos_permanencia == -1 && valor_pago == -1.0f)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE sessoes_playground SET ");
    tem_campo_anterior = 0;

    if (minutos_permanencia > 0)
    {
        strcat(query, "minutos_permanencia = ?");
        tem_campo_anterior = 1;
    }

    if (valor_pago > -1.0f)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "valor_pago = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_sessao= ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    if (minutos_permanencia > 0)
    {
        sqlite3_bind_int(statement, indice_bind++, minutos_permanencia);
    }

    if (valor_pago > -1.0f)
    {
        sqlite3_bind_double(statement, indice_bind++, valor_pago);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_sessao);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar mesa: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Sessão de Playground atualizada com sucesso!\n");
}

/**
 * Roda uma consulta de todas as sessões de playground e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_sessao, cliente.nome AS cliente, playground.nome_setor AS playground, hora_entrada, minutos_permanencia, valor_pago FROM sessoes_playground JOIN cliente ON cliente.id_cliente = sessoes_playground.id_cliente JOIN playground ON playground.id_playground = sessoes_playground.id_playground ORDER BY cliente.nome ASC, id_sessao ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Sessões de Playground cadastradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar sessões de playground e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da sessão de playground selecionada.
 */
int sessao_play_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_sessao = 0, opcao;
    sqlite3_int64 id_nova_sessao = 0;

    while (id_sessao == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar uma sessão por cliente\n  2) buscar uma sessão por playground\n  ");
        printf("3) listar todas\n  4) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            sessao_play_buscar_por_cliente(bd);
            break;
        case 2:
            sessao_play_buscar_por_playground(bd);
            break;
        case 3:
            sessao_play_listar(bd);
            break;
        case 4:
            id_nova_sessao = sessao_play_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_nova_sessao > 0)
        {
            printf("Deseja selecionar esta sessão de playground criada? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_nova_sessao;
            }
        }

        printf("Qual o ID da sessão de playground você deseja selecionar?\n");
        id_sessao = entrada_int();
    }

    return id_sessao;
}

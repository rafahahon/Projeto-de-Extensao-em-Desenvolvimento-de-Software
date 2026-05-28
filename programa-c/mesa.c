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
 * Biblioteca Code Cat Coffee - Módulo Mesas
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "mesa.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de mesa e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_buscar(sqlite3* bd)
{
    int numero_mesa;
    sqlite3_stmt* statement = NULL;

    printf("Digite o número da mesa a buscar: ");
    numero_mesa = entrada_int();

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_mesa, numero_mesa, tomada, extra_monitor FROM mesa WHERE numero_mesa = ? ORDER BY numero_mesa ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    sqlite3_bind_int(statement, 1, numero_mesa);

    printf("Mesas encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra uma nova mesa.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da mesa se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 mesa_cadastrar(sqlite3* bd)
{
    int numero_mesa, tomadas, monitores_extra, retorno;
    sqlite3_int64 id_mesa;
    sqlite3_stmt* statement = NULL;

    printf("Digite o número da mesa:\n");
    numero_mesa = entrada_int();

    while (numero_mesa <= 0)
    {
        printf("Número da mesa deve ser maior que zero, digite um número válido:\n");
        numero_mesa = entrada_int();
    }

    printf("Digite o número de tomadas disponíveis na mesa:\n");
    tomadas = entrada_int();

    while (tomadas < 0)
    {
        printf("Número de tomadas deve ser maior ou igual a zero, digite um número válido:\n");
        tomadas = entrada_int();
    }

    printf("Digite o número de monitores extras disponíveis na mesa:\n");
    monitores_extra = entrada_int();

    while (monitores_extra < 0)
    {
        printf("Número de monitores extras deve ser maior ou igual a zero, digite um número válido:\n");
        monitores_extra = entrada_int();
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO mesa(numero_mesa, tomada, extra_monitor) VALUES (?, ?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int(statement, 1, numero_mesa);
    sqlite3_bind_int(statement, 2, tomadas);
    sqlite3_bind_int(statement, 3, monitores_extra);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID da mesa, para referência
    id_mesa = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Mesa cadastrada!\n");

    return id_mesa;
}

/**
 * Edita uma mesa existente.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_editar(sqlite3* bd)
{
    char query[1024];
    int numero_mesa, tomadas, monitores_extra, retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_mesa = 0;
    sqlite3_stmt* statement = NULL;

    id_mesa = mesa_selecionar(bd);

    if (id_mesa == 0)
    {
        printf("Nenhuma mesa selecionada! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo número da mesa ou enter para pular:\n");
    numero_mesa = entrada_int();

    while (numero_mesa != -1 && numero_mesa <= 0)
    {
        printf("Número da mesa deve ser maior que zero, digite um número válido:\n");
        numero_mesa = entrada_int();
    }

    printf("Digite o novo número de tomadas disponíveis na mesa:\n");
    tomadas = entrada_int();

    while (tomadas < -1)
    {
        printf("Número de tomadas deve ser maior ou igual a zero, digite um número válido:\n");
        tomadas = entrada_int();
    }

    printf("Digite o número de monitores extras disponíveis na mesa:\n");
    monitores_extra = entrada_int();

    while (monitores_extra < -1)
    {
        printf("Número de monitores extras deve ser maior ou igual a zero, digite um número válido:\n");
        monitores_extra = entrada_int();
    }

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (numero_mesa == -1 && tomadas == -1 && monitores_extra == -1)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE mesa SET ");
    tem_campo_anterior = 0;

    if (numero_mesa > 0)
    {
        strcat(query, "numero_mesa = ?");
        tem_campo_anterior = 1;
    }

    if (tomadas > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "tomada = ?");
        tem_campo_anterior = 1;
    }

    if (monitores_extra > -1)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "extra_monitor = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_mesa = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    if (numero_mesa > 0)
    {
        sqlite3_bind_int(statement, indice_bind++, numero_mesa);
    }

    if (tomadas > -1)
    {
        sqlite3_bind_int(statement, indice_bind++, tomadas);
    }

    if (monitores_extra > -1)
    {
        sqlite3_bind_int(statement, indice_bind++, monitores_extra);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_mesa);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar mesa: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Mesa atualizada com sucesso!\n");
}

/**
 * Roda uma consulta de todas as mesas e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_mesa, numero_mesa, tomada, extra_monitor FROM mesa ORDER BY numero_mesa ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Mesas cadastradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar mesas e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da mesa selecionada.
 */
int mesa_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_mesa = 0, opcao;
    sqlite3_int64 id_nova_mesa = 0;

    while (id_mesa == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar uma mesa\n  2) listar todas\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            mesa_buscar(bd);
            break;
        case 2:
            mesa_listar(bd);
            break;
        case 3:
            id_nova_mesa = mesa_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_nova_mesa > 0)
        {
            printf("Deseja selecionar esta mesa criada? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_nova_mesa;
            }
        }

        printf("Qual o ID da mesa você deseja selecionar?\n");
        id_mesa = entrada_int();
    }

    return id_mesa;
}

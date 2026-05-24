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
 * Biblioteca Code Cat Coffee - Módulo Reservas de Mesa
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bd.h"
#include "cliente.h"
#include "mesa.h"
#include "reserva_mesa.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de reserva de mesa por nome de cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_buscar_por_cliente(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do cliente para buscar a reserva de mesa: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_reserva, cliente.nome AS cliente, mesa.numero_mesa, inicio_reserva, fim_reserva FROM reserva_mesa JOIN cliente ON cliente.id_cliente = reserva_mesa.id_cliente JOIN mesa ON mesa.id_mesa = reserva_mesa.id_mesa WHERE cliente.nome = ? ORDER BY inicio_reserva ASC;",
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

    printf("Reservas de Mesa encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Roda uma consulta de busca de reserva de mesa pelo número da mesa e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_buscar_por_mesa(sqlite3* bd)
{
    int numero_mesa;
    sqlite3_stmt* statement = NULL;

    printf("Digite o número da mesa reservada a buscar: ");
    numero_mesa = entrada_int();

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_reserva, cliente.nome AS cliente, mesa.numero_mesa, inicio_reserva, fim_reserva FROM reserva_mesa JOIN cliente ON cliente.id_cliente = reserva_mesa.id_cliente JOIN mesa ON mesa.id_mesa = reserva_mesa.id_mesa WHERE mesa.numero_mesa = ? ORDER BY inicio_reserva ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    sqlite3_bind_int(statement, 1, numero_mesa);

    printf("Reservas de Mesa encontradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra uma nova reserva de mesa.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da reserva de mesa se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 reserva_mesa_cadastrar(sqlite3* bd)
{
    int *inicio_reserva, *fim_reserva, retorno;
    struct tm dataHelper = {0};
    time_t epoch_inicio, epoch_fim;
    sqlite3_int64 id_cliente, id_mesa;
    sqlite3_stmt* statement = NULL;

    id_cliente = cliente_selecionar(bd);

    id_mesa = mesa_selecionar(bd);

    printf("Data de início (DD-MM-AAAA ou vazio para hoje): ");
    entrada_data(inicio_reserva);

    if (inicio_reserva == NULL)
    {
        printf("Erro ao obter data.\n");
        return 0;
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
    dataHelper.tm_mday = inicio_reserva[0];
    dataHelper.tm_mon = inicio_reserva[1] - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = inicio_reserva[2] - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    epoch_inicio = mktime(&dataHelper);

    if (epoch_inicio == -1)
    {
        printf("Erro ao converter data.\n");
        return 0;
    }

    printf("Data de fim da reserva (DD-MM-AAAA ou vazio para hoje): ");
    entrada_data(fim_reserva);

    if (fim_reserva == NULL)
    {
        printf("Erro ao obter data.\n");
        return 0;
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
    dataHelper.tm_mday = fim_reserva[0];
    dataHelper.tm_mon = fim_reserva[1] - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = fim_reserva[2] - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    epoch_fim = mktime(&dataHelper);

    if (epoch_fim == -1)
    {
        printf("Erro ao converter data.\n");
        return 0;
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO reserva_mesa(id_cliente, id_mesa, inicio_reserva, fim_reserva) VALUES (?, ?, ?, ?);",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar reserva de mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int64(statement, 1, id_cliente);
    sqlite3_bind_int64(statement, 2, id_mesa);
    sqlite3_bind_int64(statement, 3, epoch_inicio);
    sqlite3_bind_int64(statement, 4, epoch_fim);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar reserva de mesa: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID da reserva de mesa, para referência
    id_mesa = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Reserva de Mesa cadastrada!\n");

    return id_mesa;
}

/**
 * Edita uma reserva de mesa existente.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_editar(sqlite3* bd)
{
    char query[1024];
    int *inicio_reserva, *fim_reserva, retorno, tem_campo_anterior, indice_bind;
    struct tm dataHelper = {0};
    time_t epoch_inicio, epoch_fim;
    sqlite3_int64 id_reserva = 0;
    sqlite3_stmt* statement = NULL;

    id_reserva = reserva_mesa_selecionar(bd);

    if (id_reserva == 0)
    {
        printf("Nenhuma reserva de mesa selecionada! Cancelando edição.\n");
        return;
    }

    printf("Digite a nova data de início (DD-MM-AAAA ou vazio para hoje): ");
    entrada_data(inicio_reserva);

    if (inicio_reserva == NULL)
    {
        printf("Erro ao obter data.\n");
        return;
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
    dataHelper.tm_mday = inicio_reserva[0];
    dataHelper.tm_mon = inicio_reserva[1] - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = inicio_reserva[2] - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    epoch_inicio = mktime(&dataHelper);

    if (epoch_inicio == -1)
    {
        printf("Erro ao converter data.\n");
        return;
    }

    printf("Digite a nova data de fim da reserva (DD-MM-AAAA ou vazio para hoje): ");
    entrada_data(fim_reserva);

    if (fim_reserva == NULL)
    {
        printf("Erro ao obter data.\n");
        return;
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
    dataHelper.tm_mday = fim_reserva[0];
    dataHelper.tm_mon = fim_reserva[1] - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = fim_reserva[2] - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    epoch_fim = mktime(&dataHelper);

    if (epoch_fim == -1)
    {
        printf("Erro ao converter data.\n");
        return;
    }

    // Verifica se o usuário simplesmente apertou enter em tudo
    if (inicio_reserva == NULL && fim_reserva == NULL)
    {
        printf("Nenhum dado fornecido, edição cancelada.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE reserva_mesa SET ");
    tem_campo_anterior = 0;

    if (epoch_inicio > 0)
    {
        strcat(query, "inicio_reserva = ?");
        tem_campo_anterior = 1;
    }

    if (epoch_fim > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "fim_reserva = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_reserva = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar produto: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (epoch_inicio > 0)
    {
        sqlite3_bind_int64(statement, indice_bind++, epoch_inicio);
    }

    if (epoch_fim > 0)
    {
        sqlite3_bind_int64(statement, indice_bind++, epoch_fim);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_reserva);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar reserva de mesa: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Reserva de Mesa atualizada com sucesso!\n");
}

/**
 * Roda uma consulta de todas as reservas de mesa e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_reserva, cliente.nome AS cliente, mesa.numero_mesa AS numero_mesa, inicio_reserva, fim_reserva FROM reserva_mesa JOIN cliente ON cliente.id_cliente = reserva_mesa.id_cliente JOIN mesa ON mesa.id_mesa = reserva_mesa.id_mesa ORDER BY inicio_reserva ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Reservas de Mesa cadastradas:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar reservas de mesa e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da reserva de mesa selecionada.
 */
int reserva_mesa_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_mesa = 0, opcao;
    sqlite3_int64 id_nova_mesa = 0;

    while (id_mesa == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar uma reserva por cliente\n  2) buscar uma reserva por mesa\n  ");
        printf("3) listar todas\n  4) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            reserva_mesa_buscar_por_cliente(bd);
            break;
        case 2:
            reserva_mesa_buscar_por_mesa(bd);
            break;
        case 3:
            reserva_mesa_listar(bd);
            break;
        case 4:
            id_nova_mesa = reserva_mesa_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_nova_mesa > 0)
        {
            printf("Deseja selecionar esta reserva de mesa criada? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_nova_mesa;
            }
        }

        printf("Qual o ID da reserva de mesa você deseja selecionar?\n");
        id_mesa = entrada_int();
    }

    return id_mesa;
}

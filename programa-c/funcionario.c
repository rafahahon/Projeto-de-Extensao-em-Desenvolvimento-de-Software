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
 * Biblioteca Code Cat Coffee - Módulo Funcionários
 */

#include <stdio.h>
#include <string.h>
#include "bd.h"
#include "funcionario.h"
#include "utilidades.h"

/**
 * Roda uma consulta de busca de funcionário e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_buscar(sqlite3* bd)
{
    char nome[255], termo_busca[300];
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do funcionário a buscar: ");
    entrada_string(nome, sizeof(nome));

    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_fun, nome, cargo FROM funcionario WHERE nome LIKE ? ORDER BY nome ASC;",
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

    printf("Funcionários encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Cadastra um novo funcionário.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do funcionário se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 funcionario_cadastrar(sqlite3* bd)
{
    char nome[255], cargo[255];
    int retorno;
    sqlite3_int64 id_fun;
    sqlite3_stmt* statement = NULL;

    printf("Digite o nome do funcionário:\n");
    entrada_string(nome, sizeof(nome));

    while (valida_string(nome, 3, 1) == 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite o cargo do funcionário:\n");
    entrada_string(cargo, sizeof(cargo));

    while (valida_string(cargo, 5, 1) == 0)
    {
        printf("Cargo não pode ser vazio ou menor que 5 letras, digite um cargo válido:\n");
        entrada_string(cargo, sizeof(cargo));
    }

    retorno = bd_prepara_consulta(
        bd,
        "INSERT INTO funcionario(nome, cargo) VALUES (?, ?)",
        &statement
    );

    if (retorno != 0)
    {
        printf("Erro ao cadastrar funcionário: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, cargo, -1, SQLITE_TRANSIENT);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao cadastrar funcionário: %s\n", sqlite3_errmsg(bd));
        return 0;
    }

    // Pega o ID do funcionário, para referência
    id_fun = sqlite3_last_insert_rowid(bd);

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Funcionário cadastrado!\n");

    return id_fun;
}

/**
 * Edita um funcionário existente.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_editar(sqlite3* bd)
{
    char nome[255], cargo[255], query[1024];
    int retorno, tem_campo_anterior, indice_bind;
    sqlite3_int64 id_fun = 0;
    sqlite3_stmt* statement = NULL;

    id_fun = funcionario_selecionar(bd);

    if (id_fun == 0)
    {
        printf("Nenhum funcionário selecionado! Cancelando edição.\n");
        return;
    }

    printf("Digite o novo nome do funcionário ou enter para pular:\n");
    entrada_string(nome, sizeof(nome));

    while (strlen(nome) > 0 && valida_string(nome, 3, 1) <= 0)
    {
        printf("Nome não pode ser vazio ou menor que 3 letras, digite um nome válido:\n");
        entrada_string(nome, sizeof(nome));
    }

    printf("Digite o novo cargo do funcionário ou enter para pular:\n");
    entrada_string(cargo, sizeof(cargo));

    while (strlen(cargo) > 0 && valida_string(cargo, 5, 1) <= 0)
    {
        printf("Cargo não pode ser vazio ou menor que 5 letras, digite um cargo válido:\n");
        entrada_string(cargo, sizeof(cargo));
    }

    if (strlen(nome) == 0 && strlen(cargo) == 0)
    {
        printf("Nenhum dado fornecido, cancelando edição.\n");
        return;
    }

    // Construção dinâmica da query
    strcpy(query, "UPDATE funcionario SET ");
    tem_campo_anterior = 0;

    if (strlen(nome) > 0)
    {
        strcat(query, "nome = ?");
        tem_campo_anterior = 1;
    }

    if (strlen(cargo) > 0)
    {
        if (tem_campo_anterior) strcat(query, ", ");
        strcat(query, "cargo = ?");
    }

    // Adiciona a condição final
    strcat(query, " WHERE id_fun = ?;");

    retorno = bd_prepara_consulta(bd, query, &statement);

    if (retorno != 0)
    {
        printf("Erro ao atualizar funcionário: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Vamos iniciar o índice em 1 e entao incrementando conforme executamos cada bind
    indice_bind = 1;

    // Usamos SQLITE_TRANSIENT, pois as variáveis são arrays locais que serão destruídas
    if (strlen(nome) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, nome, -1, SQLITE_TRANSIENT);
    }

    if (strlen(cargo) > 0)
    {
        sqlite3_bind_text(statement, indice_bind++, cargo, -1, SQLITE_TRANSIENT);
    }

    // O ID será sempre o último indice_bind disponível
    sqlite3_bind_int64(statement, indice_bind, id_fun);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao atualizar funcionário: %s\n", sqlite3_errmsg(bd));
        return;
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Funcionário atualizado com sucesso!\n");
}

/**
 * Roda uma consulta de todos os funcionários e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_listar(sqlite3* bd)
{
    sqlite3_stmt* statement = NULL;
    const int retorno = bd_prepara_consulta(
        bd,
        "SELECT id_fun, nome, cargo FROM funcionario ORDER BY nome ASC;",
        &statement
    );

    if (retorno != 0)
    {
        return;
    }

    printf("Funcionários cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuário a opção de buscar ou listar funcionários e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do funcionário selecionado.
 */
int funcionario_selecionar(sqlite3* bd)
{
    char confirmacao[5];
    int id_fun = 0, opcao;
    sqlite3_int64 id_novo_fun = 0;

    while (id_fun == 0)
    {
        printf("Selecione a opção desejada:\n  ");
        printf("1) buscar um funcionários\n  2) listar todos\n  3) cadastrar\n");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            funcionario_buscar(bd);
            break;
        case 2:
            funcionario_listar(bd);
            break;
        case 3:
            id_novo_fun = funcionario_cadastrar(bd);
            break;
        default:
            printf("Opção inválida!\n");
            continue;
        }

        if (id_novo_fun > 0)
        {
            printf("Deseja selecionar este funcionário criado? [s/n]\n");
            entrada_string(confirmacao, sizeof(confirmacao));

            if (strcmp(confirmacao, "s") == 0)
            {
                return (int)id_novo_fun;
            }
        }

        printf("Qual o ID do funcionário você deseja selecionar?\n");
        id_fun = entrada_int();
    }

    return id_fun;
}

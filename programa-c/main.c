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
 * Olá, pro! Nosso programa em C está ligado ao nosso banco de dados!
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "sqlite3.h" // para trabalhar com o SQLite em C, precisamos adicionar a sua biblioteca
#include <string.h>

// Aqui nós declaramos a referência ao banco de dados como "bd" para facilitar na hora de chamar
sqlite3* bd;
// variável para armazenar o retorno das funções chamadas do SQLite
int retorno;

// Aqui criamos a função Conectar, que faz a conexão do código com o banco
void conectar()
{
    setlocale(LC_ALL, "Portuguese");
    // Aqui nós iniciamos a conexão com o banco de dados
    retorno = sqlite3_open_v2(
        "file://./../banco-de-dados/codecatcoffee.sqlite",
        &bd,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_PRIVATECACHE,
        "win32"
    );

    // Aqui nós vamos testar se a conexão com o banco de dados deu certo, e caso não tenha dado,
    // vai mostrar a mensagem "Erro na conexão"
    if (retorno != SQLITE_OK)
    {
        printf("Erro na conexão ao banco de dados: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        exit(1);
    }

    // caso a conexão tenha dado certo, essa será a mensagem exibida
    printf("Conectado ao banco!\n");
}

/* aqui criamos a função cadastrar produto, onde vamos pedir e salvar os valores dos atributos da tabela Produto,
   cadastrando um novo produto */
void cadastrarProduto()
{
    setlocale(LC_ALL, "Portuguese");
    char *nome, *query;
    float preco;
    int quantidade, categoria;
    sqlite3_stmt* statement;

    // aqui nos pedimos para o usuário o nome do produto
    printf("Nome do produto: ");
    // aqui o código lê o que foi digitado
    scanf("%s", &nome);

    // aqui pedimos o preço
    printf("Preço: ");
    scanf("%f", &preco);

    // aqui pedimos a quantidade
    printf("Quantidade: ");
    scanf("%d", &quantidade);

    // aqui pedimos a categoria
    printf("Selecione a categoria: ");
    scanf("%d", &categoria);
    // TODO: ALTERAR PARA PEGAR AS CATEGORIAS DO BD E LISTAR. USUARIO DEVE DIGITAR O ID DA CATEGORIA

    query = "INSERT INTO produto(nome, preco, quantidade, categoria) VALUES (?, ?, ?, ?)";

    retorno = sqlite3_prepare_v3(
        bd, /* Referencia do banco de dados */
        query, /* Consulta SQL, UTF-8 encoded */
        -1, /* Tamanho máximo da consulta em bytes. */
        SQLITE_PREPARE_FROM_DDL | SQLITE_PREPARE_NORMALIZE, /* Zero ou mais flags SQLITE_PREPARE_ */
        &statement, /* SAIDA: Referencia a statement */
        NULL /* SAIDA: Ponteiro para a parte nao usada da query */
    );

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao preparar a consulta: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        exit(1);
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
        printf("Erro ao executar a consulta: %s\n", sqlite3_errmsg(bd));
        sqlite3_finalize(statement);
        sqlite3_close(bd);
        exit(1);
    }

    // Limpeza pós-execução
    sqlite3_finalize(statement);

    printf("Produto cadastrado!\n");
}

void cadastrarPedido()
{
    setlocale(LC_ALL, "Portuguese");
    char data[11], *query;
    int cliente, produto, quantidade, dia, mes, ano, retorno;
    float total = 0.0;
    struct tm dataHelper = {0};
    time_t agora = time(NULL);
    sqlite3_stmt* statement;

    printf("Cliente: ");
    // TODO: Listar os clientes
    scanf("%d", &cliente);

    printf("Produto: ");
    // TODO: listar os produtos
    scanf("%d", &produto);

    printf("Quantidade: ");
    scanf("%f", &quantidade);

    printf("Data do pedido (DD-MM-AAAA ou vazio para hoje): ");
    scanf("%s", &data);

    if (strcmp(data, "\n") != 0)
    {
        strftime(data, sizeof data, "%d-%m-%Y", localtime(&agora));
    }

    sscanf(data, "%d-%d-%d", &dia, &mes, &ano);
    dataHelper.tm_mday = dia;
    dataHelper.tm_mon = mes - 1; // Meses vão de 0 a 11, tem que subtrair 1
    dataHelper.tm_year = ano - 1900; // Precisa subtrair, pq o epoch do C vai de 1900 até hoje
    dataHelper.tm_isdst = -1; // Deixa o sistema arrumar para horario de verão
    time_t epoch = mktime(&dataHelper);

    if (epoch == -1)
    {
        printf("Erro ao converter data.\n");
        sqlite3_close(bd);
        exit(1);
    }

    // total = quantidade * valor_unitario;

    query =
        "INSERT INTO pedido(id_cliente, id_func, id_forma_pagto, data_pedido, valor_total_pedido) VALUES (?, ?, ?, ?, ?)";

    retorno = sqlite3_prepare_v3(
        bd, /* Referencia do banco de dados */
        query, /* Consulta SQL, UTF-8 encoded */
        -1, /* Tamanho máximo da consulta em bytes. */
        SQLITE_PREPARE_FROM_DDL | SQLITE_PREPARE_NORMALIZE, /* Zero ou mais flags SQLITE_PREPARE_ */
        &statement, /* SAIDA: Referencia a statement */
        NULL /* SAIDA: Ponteiro para a parte nao usada da query */
    );

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao preparar a consulta: %s\n", sqlite3_errmsg(bd));
        sqlite3_close(bd);
        exit(1);
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int(statement, 1, cliente);
    sqlite3_bind_int(statement, 2, 1);
    sqlite3_bind_int(statement, 3, 1);
    sqlite3_bind_int64(statement, 4, epoch);
    sqlite3_bind_double(statement, 5, total);

    // Rodamos a consulta
    retorno = sqlite3_step(statement);

    if (retorno != SQLITE_DONE)
    {
        printf("Erro ao criar o pedido: %s\n", sqlite3_errmsg(bd));
        sqlite3_finalize(statement);
        sqlite3_close(bd);
        exit(1);
    }

    // TODO: cadastrar o item no pedido

    printf("Pedido cadastrado!\n");
}

static int listarPedidosCallback(void* naoUsado, int numColunas, char** valores, char** nomeColuna)
{
    setlocale(LC_ALL, "Portuguese");
    for (int i = 0; i < numColunas; i++)
    {
        printf("%s: %s |", nomeColuna[i], valores[i] ? valores[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void listarPedidos()
{
    setlocale(LC_ALL, "Portuguese");
    char* erro;
    int retorno = sqlite3_exec(bd, "SELECT * FROM pedido", listarPedidosCallback, 0, &erro);

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao selecionar os pedidos: %s\n", sqlite3_errmsg(bd));
        sqlite3_free(erro);
        sqlite3_close(bd);
        exit(1);
    }

    sqlite3_close(bd);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    conectar();

    // TODO: login de funcionario?

    do
    {
        printf("\n Code Cat Coffee ^. .^\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Cadastrar pedidos\n");
        printf("3 - Listar pedidos\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cadastrarProduto();
            break;

        case 2:
            cadastrarPedido();
            break;

        case 3:
            listarPedidos();
            break;

        case 4:
            printf("Tchau tchau!\n");
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 4);

    sqlite3_close(bd);

    return 0;
}

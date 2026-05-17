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
 * Olá, pro! O nosso programa em C está ligado ao nosso banco de dados!
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h" // para trabalhar com o SQLite em C, precisamos adicionar a sua biblioteca

/*
 * Aqui criamos variáveis em comum para uso no programa
 */
int retorno; // Para armazenar o retorno das funções chamadas do SQLite
char* erro; // Para armazenar o último erro do BD
sqlite3_stmt* statement; // Para armazenar prepared statements
sqlite3* bd; // Referência ao banco de dados como "bd" para facilitar na hora de chamar

/**
 * Lê um arquivo no sistema de arquivos atual e retorna o seu conteúdo.
 * @param caminho_arquivo O caminho para o arquivo a ser lido.
 * @return O conteúdo do arquivo.
 */
char* ler_arquivo(const char* caminho_arquivo)
{
    char* conteudo;
    // Aqui abrimos o arquivo para leitura
    FILE* arquivo = fopen(caminho_arquivo, "rb");

    if (arquivo == NULL)
    {
        fprintf(stderr, "Não foi possível abrir o arquivo.\n");
        exit(1);
    }

    // Aqui pegamos o tamanho total do arquivo para usar em seguida
    fseek(arquivo, 0, SEEK_END);
    const long tam_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    // Aqui colocamos o conteúdo de cada linha do arquivo dentro da variável conteudo
    fread(&conteudo, 1, tam_arquivo, arquivo);
    conteudo[tam_arquivo] = 0; // Finaliza a string com um null

    // Fechando o arquivo para liberar memória
    fclose(arquivo);

    return conteudo;
}

/**
 * Inicializa o Banco de Dados com a estrutura básica de tabelas para o programa.
 */
void bd_criar_tabelas()
{
    // Vamos ler o arquivo SQL num buffer de string
    char* query = ler_arquivo("./../banco-de-dados/esquema-sqlite.sql");

    // Executa a query
    retorno = sqlite3_exec(bd, query, NULL, 0, &erro);

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao inicializar o banco de dados: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(bd);
        exit(1);
    }

    printf("Banco de dados inicializado com sucesso!\n");
}

/**
 * Preenche as tabelas do Banco de Dados com dados de exemplo.
 */
void bd_popular_tabelas()
{
    // Vamos ler o arquivo SQL num buffer de string
    char* query = ler_arquivo("./../banco-de-dados/esquema-sql-inserção.sql");

    // Executa a query
    retorno = sqlite3_exec(bd, query, NULL, 0, &erro);

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao popular o banco de dados: %s\n", erro);
        sqlite3_free(erro);
        return;
    }

    printf("Banco de dados populado com sucesso!\n");
}

/**
 * Verifica a integridade do Banco de Dados através das tabelas presentes.
 */
void bd_verificar_integridade()
{
    char *query,
         confirmacao,
         *tabelasEsperadas[12] = {
             "cliente", "funcionario", "mesa", "reserva_mesa", "playground", "sessoes_playground", "gato",
             "produto", "categoria_produto", "pedido", "itens_pedido", "forma_pagto"
         };
    int tabelasRetornadas[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    query = "SELECT name FROM sqlite_schema WHERE type ='table' AND name NOT LIKE 'sqlite_%';";
    retorno = sqlite3_prepare_v2(bd, query, -1, &statement, 0);

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao verificar integridade do banco de dados: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(bd);
        exit(1);
    }

    // Vamos agora verificar linha a linha dos resultados e popular se todas as tabelas esperadas foram retornadas
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        // Coluna 0 é o 'name' da nossa consulta SELECT
        const char* nomeTabela = (const char*)sqlite3_column_text(statement, 0);

        if (nomeTabela == NULL) continue; // Verificação de segurança

        for (int i = 0; i < 12; i++)
        {
            // strcmp retorna 0 se as strings forem idênticas
            if (strcmp(tabelasEsperadas[i], nomeTabela) == 0)
            {
                tabelasRetornadas[i] = 1;
                break;
            }
        }
    }

    sqlite3_finalize(statement);

    for (int i = 0; i < 12; i++)
    {
        if (tabelasRetornadas[i] == 0)
        {
            printf("Tabela %s não está presente no banco de dados. ", tabelasEsperadas[i]);
            printf("Deseja inicializar as tabelas no banco de dados? [s/n]\n");
            scanf("%s", &confirmacao);

            if (confirmacao == 's')
            {
                bd_criar_tabelas();

                printf("Deseja popular as tabelas com dados de exemplo? [s/n]\n");
                scanf("%s", &confirmacao);

                if (confirmacao == 's')
                {
                    bd_popular_tabelas();
                }

                break;
            }
        }
    }
}

/**
 * Conecta com o banco de dados SQLite para uso durante a execução.
 */
void bd_conectar()
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

    // Caso a conexão tenha dado certo, essa será a mensagem exibida
    printf("Conectado ao banco!\n");

    bd_verificar_integridade();
}

/**
 * Roda uma consulta select e imprime os nomes das colunas e cada linha no resultado.
 * @param stmt A consulta preparada (prepared statement)
 */
void bd_imprimir_resultados_tabela(sqlite3_stmt* stmt)
{
    int total_col = sqlite3_column_count(stmt);
    int i;

    // 1. Mostra o cabeçalho
    printf("\n");

    for (i = 0; i < total_col; i++)
    {
        // %-15s é: string, alinhada a esquerda (COMUNISTA!!), 15 caracteres de largura
        printf("%-15s | ", sqlite3_column_name(stmt, i));
    }

    printf("\n");

    // 2. Separador de linhas/colunas
    for (i = 0; i < total_col; i++)
    {
        printf("----------------|");
    }

    printf("\n");

    // 3. Mostra as linhas
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        for (i = 0; i < total_col; i++)
        {
            const char* val = (const char*)sqlite3_column_text(stmt, i);
            printf("%-15s | ", val ? val : "NULL");
        }

        printf("\n");
    }

    printf("\n");
}

int bd_prepara_consulta(char* query)
{
    retorno = sqlite3_prepare_v3(
        bd, /* Referência do banco de dados */
        query, /* Consulta SQL, UTF-8 encoded */
        -1, /* Tamanho máximo da consulta em bytes. */
        SQLITE_PREPARE_FROM_DDL | SQLITE_PREPARE_NORMALIZE, /* Zero ou mais flags SQLITE_PREPARE_ */
        &statement, /* SAIDA: Referencia a statement */
        NULL /* SAIDA: Ponteiro para a parte nao usada da query */
    );

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao preparar a consulta: %s\n", sqlite3_errmsg(bd));
        return 1;
    }

    return 0;
}

/* aqui criamos a função cadastrar produto, onde vamos pedir e salvar os valores dos atributos da tabela Produto,
   cadastrando um novo produto */
void cadastrarProduto()
{
    setlocale(LC_ALL, "Portuguese");
    char* nome;
    float preco;
    int quantidade, categoria;

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

    retorno = bd_prepara_consulta("INSERT INTO produto(nome, preco, quantidade, categoria) VALUES (?, ?, ?, ?)");

    if (retorno != 0)
    {
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

/**
 * Roda uma consulta de busca de clientes e imprime o resultado da busca.
 */
void cliente_buscar()
{
    setlocale(LC_ALL, "Portuguese");
    char* nome;

    printf("Digite o nome cliente a buscar: ");
    scanf("%s", &nome);

    retorno = bd_prepara_consulta("SELECT id_cliente, nome FROM cliente WHERE nome LIKE '%?%' ORDER BY nome ASC;");

    if (retorno != 0)
    {
        sqlite3_close(bd);
        exit(1);
    }

    sqlite3_bind_text(statement, 1, nome, -1, SQLITE_STATIC);

    printf("Clientes encontrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 */
void cliente_listar()
{
    setlocale(LC_ALL, "Portuguese");

    retorno = bd_prepara_consulta("SELECT id_cliente, nome FROM cliente ORDER BY nome ASC;");

    if (retorno != 0)
    {
        sqlite3_close(bd);
        exit(1);
    }

    printf("Clientes cadastrados:\n");

    bd_imprimir_resultados_tabela(statement);

    // Limpeza pós-execução
    sqlite3_finalize(statement);
}

/**
 * Dá ao usuario a opção de buscar ou listar clientes e pede para o usuário selecionar um por ID.
 * @return ID do cliente selecionado.
 */
int cliente_selecionar()
{
    setlocale(LC_ALL, "Portuguese");
    int cliente = 0, opcao;

    while (cliente == 0)
    {
        printf("Selecione a opção desejada:\n  1) buscar um cliente\n  2) listar todos\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            cliente_buscar();
        }
        else if (opcao == 2)
        {
            cliente_listar();
        }
        else
        {
            printf("Opção inválida!\n");
            continue;
        }

        printf("Qual cliente deseja selecionar?\n");
        scanf("%d", &cliente);
    }

    return cliente;
}

void pedido_criar()
{
    setlocale(LC_ALL, "Portuguese");
    char data[11];
    int cliente, produto, quantidade, dia, mes, ano;
    float total = 0.0;
    struct tm dataHelper = {0};
    time_t agora = time(NULL);

    cliente = cliente_selecionar();

    printf("Produto: ");
    // TODO: listar os produtos
    scanf("%d", &produto);

    printf("Quantidade: ");
    scanf("%d", &quantidade);

    printf("Data do pedido (DD-MM-AAAA ou vazio para hoje): ");
    fgets(data, sizeof(data), stdin);

    /**
     * Pra moder mexer com datas em C, precisamos usar a biblioteca de tempo do C.
     * Vamos primeiro fazer o parsing da string enviada e depois separar em dia, mes e ano.
     * Feito isso, precisamos transformar em um objeto time_t compatível com a SQLite.
     */
    if (strcmp(data, "\n") != 0) // Se a data for vazia, usar a data de hoje
    {
        strftime(data, sizeof data, "%d-%m-%Y", localtime(&agora));
    }

    // Separar a data passada e atribuir cada %d para a variável que for passada na ordem
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

    // TODO: pegar o valor do item pedido pode começar vazio ou com um item inicial e colocar como total
    // total = quantidade * valor_unitario;


    retorno = bd_prepara_consulta(
        "INSERT INTO pedido(id_cliente, id_func, id_forma_pagto, data_pedido, valor_total_pedido) VALUES (?, ?, ?, ?, ?)");

    if (retorno != 0)
    {
        sqlite3_close(bd);
        exit(1);
    }

    // Aqui adicionamos os valores de cada ? na consulta preparada, de um modo seguro
    sqlite3_bind_int(statement, 1, cliente);
    sqlite3_bind_int(statement, 2, 1); // TODO: substituir por funcionario logado?
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

    // Pega o ID do pedido, para referência
    id_pedido = sqlite3_last_insert_rowid(bd);

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

    retorno = sqlite3_exec(bd, "SELECT * FROM pedido", listarPedidosCallback, 0, &erro);

    if (retorno != SQLITE_OK)
    {
        printf("Erro ao selecionar os pedidos: %s\n", erro);
        sqlite3_free(erro);
        sqlite3_close(bd);
        exit(1);
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    bd_conectar();

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

        // TODO: cadastrar clientes
        // TODO: cadastrar funcionarios

        switch (opcao)
        {
        case 1:
            cadastrarProduto();
            break;

        case 2:
            pedido_criar();
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

    // Limpando e fechando tudo antes de finalizar
    sqlite3_finalize(statement);
    sqlite3_free(erro);
    sqlite3_close(bd);

    return 0;
}

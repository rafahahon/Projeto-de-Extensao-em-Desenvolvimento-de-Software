/**
*              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Clara Oliveira da Silva     | 3026104548 |
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Pedro Junior da Silva Soares    | 3026104213 |
 * | Rafael Baptista de Almeida      | 3026103546 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Olá, pro! Nosso programa em C está ligado ao nosso banco de dados!
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h> // para trabalhar com o MySQL em C, precisamos adicionar a sua biblioteca

// aqui nós declaramos o MySql como "bd" para facilitar na hora de chamar
MYSQL *bd;

// aqui criamos a função Conectar, que faz a conexão do código com o banco
void conectar() {
    // aqui nós iniciamos a conexão com o MySql
    bd = sqlite_open();

    /* aqui nós vamos testar se a conexão com o banco de dados deu certo, e caso não tenha dado, vai mostrar
     a mensagem "Erro na conexão" */
    if (!mysql_real_connect(bd, "localhost", "root", "senha", "cafeteria", 0, NULL, 0)) {
        printf("Erro na conexão: %s\n", mysql_error(bd));
        exit(1);
    }

    // caso a conexão tenha dado certo, essa será a mensagem exibida 
    printf("Conectado ao banco!\n");
}

/* aqui criamos a função cadastrar produto, onde vamos pedir e salvar os valores dos atributos da tabela Produto,
   cadastrando um novo produto */
void cadastrarProduto() {

    // aqui nos pedimos para o usuário o nome do produto
    printf("Nome do produto: ");
    // aqui o código lê o que foi digitado
    scanf("%s", nome);

    // aqui pedimos o preço
    printf("Preço: ");
    scanf("%f", &preco);

    // aqui pedimos a quantidade
    printf("Quantidade: ");
    scanf("%d", &quantidade);

    // aqui pedimos a categoria
    printf("Categoria: ");
    scanf("%s", &categoria);

    sprintf(query,
    "INSERT INTO produto(nome, preco, quantidade, categoria) VALUES('%s', %.2f, %d, %.s)",
    nome, preco, quantidade, categoria);

    if(mysql_query(bd, query)){
        printf("Erro: %s\n", mysql_error(bd));
    } else {
        printf("Produto cadastrado!\n");
    }
}

void cadastrarPedido() {

    printf("Cliente: ");
    scanf("%d", &cliente);

    printf("Produto: ");
    scanf("%d", &produto);

    printf("Quantidade: ");
    scanf("%f", &quantidade);

    printf("Data do pedido: ");
    scanf("%?", &data);

    total = quantidade * valor_unitario;

    sprintf(query,
    "INSERT INTO pedido(id_cliente, produto, quantidade, data_pedido, total) VALUES('%d', %d, %f, %?, %.2f)",
    cliente, produto, quantidade, data, total);

    if(mysql_query(bd, query)){
        printf("Erro: %s\n", mysql_error(bd));
    } else {
        printf("Pedido cadastrado!\n");
    }
}

void listarPedidos() {

    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(bd, "SELECT * FROM pedido");

    res = mysql_store_result(bd);

    printf("\nPedidos cadastrados:\n");

    while ((row = mysql_fetch_row(res))) {
        printf("ID: %s | Id_cliente: %d | Produto: %d | Data do pedido: %? |Quantidade: %s | Valor: %s | Total: %.2f\n",
        row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
    }

    mysql_free_result(res);
}

int main() {

    int opcao;

    conectar();

    do {

        printf("\n Code Cat Coffee ^. .^\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Cadastrar pedidos\n");
        printf("3 - Listar pedidos\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao){

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

    } while(opcao != 4);

    mysql_close(conn);

    return 0;
}
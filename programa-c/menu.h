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
 * Biblioteca Code Cat Coffee - Utilidades
 */

#ifndef CODE_CAT_COFFEE_MENU_H
#define CODE_CAT_COFFEE_MENU_H

#include "sqlite3.h"

/**
 * Imprime uma linha separadora com espaçamento de uma quebra de linha acima e abaixo dela.
 */
void imprime_linha_separadora();

/**
 * Imprime o nome e a logo da Code Cat Coffee no terminal.
 */
void imprime_logo_ccc();

/**
 * Imprime a lista de opções de um menu. Por último sempre vai colocar a opção 0 - Voltar/Sair.
 * @param opcoes Um array de strings com as opções. A primeira sempre deve ser o Voltar/Sair.
 * @param quantidade_opcoes O total de opções disponíveis na array. Deve ser calculado junto da declaração das opções.
 */
void imprime_opcoes(const char** opcoes, int quantidade_opcoes);


/**
 * Imprime o menu Clientes e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_cliente(sqlite3* bd);

/**
 * Imprime o menu Mesa e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_mesa(sqlite3* bd);

/**
 * Imprime o menu Outros e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_outros(sqlite3* bd);

/**
 * Imprime o menu Pedidos e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_pedido(sqlite3* bd);

/**
 * Imprime o menu Playgrounds e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_playground(sqlite3* bd);

/**
 * Imprime o menu Produtos e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_produto(sqlite3* bd);

/**
 * Imprime o menu Relatórios e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_relatorio(sqlite3* bd);

#endif //CODE_CAT_COFFEE_MENU_H

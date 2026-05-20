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
 * Biblioteca Code Cat Coffee - Módulo Pedido
 */

#ifndef CODE_CAT_COFFEE_PEDIDO_H
#define CODE_CAT_COFFEE_PEDIDO_H

/**
 * Adiciona um produto ao pedido.
 * @param bd A referência à conexão do banco de dados.
 * @param id_pedido ID do pedido
 */
void pedido_adicionar_item(sqlite3* bd, sqlite3_int64 id_pedido);

/**
 * Cria um pedido no Banco de Dados.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_criar(sqlite3* bd);

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_listar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_PEDIDO_H

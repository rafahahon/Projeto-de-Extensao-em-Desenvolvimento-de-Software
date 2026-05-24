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
 * @param id_pedido ID do pedido
 */
void pedido_adicionar_item(sqlite3_int64 id_pedido);

/**
 * Cria um pedido no Banco de Dados.
 */
void pedido_criar();

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 */
void pedido_listar();

#endif //CODE_CAT_COFFEE_PEDIDO_H

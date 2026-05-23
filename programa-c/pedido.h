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
 * Biblioteca Code Cat Coffee - Módulo Pedidos
 */

#ifndef CODE_CAT_COFFEE_PEDIDO_H
#define CODE_CAT_COFFEE_PEDIDO_H

#include "sqlite3.h"

/**
 * Adiciona um produto ao pedido.
 * @param bd A referência à conexão do banco de dados.
 * @param id_pedido ID do pedido
 */
void pedido_adicionar_item(sqlite3* bd, sqlite3_int64 id_pedido);

/**
 * Roda uma consulta de busca de pedidos por nome de cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_buscar_por_cliente(sqlite3* bd);

/**
 * Cria um pedido no Banco de Dados.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do pedido se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 pedido_criar(sqlite3* bd);

/**
 * Edita um pedido existente.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_editar(sqlite3* bd);

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void pedido_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar pedidos e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do pedido selecionado.
 */
int pedido_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_PEDIDO_H

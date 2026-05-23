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
 * Biblioteca Code Cat Coffee - Módulo Clientes
 */

#ifndef CODE_CAT_COFFEE_CLIENTE_H
#define CODE_CAT_COFFEE_CLIENTE_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de clientes e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void cliente_buscar(sqlite3* bd);

/**
 * Cadastra um novo cliente.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do cliente se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 cliente_cadastrar(sqlite3* bd);

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void cliente_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar clientes e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do cliente selecionado.
 */
int cliente_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_CLIENTE_H

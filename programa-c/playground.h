/**
*              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Santos Nascimento           | 3036101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026101346 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Biblioteca Code Cat Coffee - Módulo Playgrounds
 */

#ifndef CODE_CAT_COFFEE_PLAYGROUND_H
#define CODE_CAT_COFFEE_PLAYGROUND_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de playgrounds e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_buscar(sqlite3* bd);

/**
 * Cadastra um novo playground.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do playground se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 playground_cadastrar(sqlite3* bd);

/**
 * Edita um playground existente.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_editar(sqlite3* bd);

/**
 * Roda uma consulta de todos os playgrounds e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void playground_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar playgrounds e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do playground selecionado.
 */
int playground_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_PLAYGROUND_H

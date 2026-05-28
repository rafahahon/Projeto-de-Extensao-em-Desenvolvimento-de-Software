/**
*              **Grupo HttpCats**
 * ================================================
 * | Nome                            | RA         |
 * ================================================
 * | Ana Santos Nascimento           | 3026101499 |
 * | Breno Fessel Da Paz             | 3026104340 |
 * | Gabrielli Minhano Barbosa       | 3026103592 |
 * | Rafael Baptista de Almeida      | 3026101346 |
 * | Rafaella Pinto Hahon            | 3026102787 |
 * | Victor Nunes Garcia             | 3026101023 |
 * ================================================
 *
 * Biblioteca Code Cat Coffee - Módulo Gatos
 */

#ifndef CODE_CAT_COFFEE_GATO_H
#define CODE_CAT_COFFEE_GATO_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de gatos e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_buscar(sqlite3* bd);

/**
 * Cadastra um novo gato.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID do gato se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 gato_cadastrar(sqlite3* bd);

/**
 * Edita um gato existente.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_editar(sqlite3* bd);

/**
 * Roda uma consulta de todos os gatos e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void gato_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar gatos e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do gato selecionado.
 */
int gato_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_GATO_H

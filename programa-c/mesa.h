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
 * Biblioteca Code Cat Coffee - Módulo Mesas
 */

#ifndef CODE_CAT_COFFEE_MESA_H
#define CODE_CAT_COFFEE_MESA_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de mesa e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_buscar(sqlite3* bd);

/**
 * Cadastra uma nova mesa.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da mesa se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 mesa_cadastrar(sqlite3* bd);

/**
 * Edita uma mesa existente.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_editar(sqlite3* bd);

/**
 * Roda uma consulta de todas as mesas e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void mesa_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar mesas e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da mesa selecionada.
 */
int mesa_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_MESA_H

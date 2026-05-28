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
 * Biblioteca Code Cat Coffee - Módulo Sessões de Playground
 */

#ifndef CODE_CAT_COFFEE_SESSAO_PLAYGROUND_H
#define CODE_CAT_COFFEE_SESSAO_PLAYGROUND_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de sessão de playground por cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_buscar_por_cliente(sqlite3* bd);

/**
 * Roda uma consulta de busca de sessão de playground por playground e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_buscar_por_playground(sqlite3* bd);

/**
 * Cadastra uma nova sessão de playground.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da sessão de playground se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 sessao_play_cadastrar(sqlite3* bd);

/**
 * Edita uma sessão de playground existente.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_editar(sqlite3* bd);

/**
 * Roda uma consulta de todas as sessões de playground e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void sessao_play_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar sessões de playground e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da sessão de playground selecionada.
 */
int sessao_play_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_SESSAO_PLAYGROUND_H

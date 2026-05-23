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
 * Biblioteca Code Cat Coffee - Módulo Formas de Pagamento
 */

#ifndef CODE_CAT_COFFEE_FORMA_PAGAMENTO_H
#define CODE_CAT_COFFEE_FORMA_PAGAMENTO_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de forma de pagamento e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_buscar(sqlite3* bd);

/**
 * Cadastra uma nova forma de pagamento.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da forma de pagamento se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 forma_pagto_cadastrar(sqlite3* bd);

/**
 * Edita uma forma de pagamento existente.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_editar(sqlite3* bd);

/**
 * Roda uma consulta de todas as formas de pagamento e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void forma_pagto_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar formas de pagamento e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da forma de pagamento selecionada.
 */
int forma_pagto_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_FORMA_PAGAMENTO_H

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
 * Biblioteca Code Cat Coffee - Módulo Funcionários
 */

#ifndef CODE_CAT_COFFEE_FUNCIONARIO_H
#define CODE_CAT_COFFEE_FUNCIONARIO_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de funcionário e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_buscar(sqlite3* bd);

/**
 * Cadastra uma nova funcionário.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da funcionário se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 funcionario_cadastrar(sqlite3* bd);

/**
 * Edita um funcionário existente.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_editar(sqlite3* bd);

/**
 * Roda uma consulta de todos os funcionários e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void funcionario_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar funcionários e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID do funcionário selecionado.
 */
int funcionario_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_FUNCIONARIO_H

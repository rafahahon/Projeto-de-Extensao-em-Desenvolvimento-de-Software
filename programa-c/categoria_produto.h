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
 * Biblioteca Code Cat Coffee - Módulo Categoria de Produtos
 */

#ifndef CODE_CAT_COFFEE_CATEGORIA_PRODUTO_H
#define CODE_CAT_COFFEE_CATEGORIA_PRODUTO_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de categoria de produto e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void cat_prod_buscar(sqlite3* bd);

/**
 * Cadastra uma nova categoria de produto.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da categoria de produto se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 cat_prod_cadastrar(sqlite3* bd);

/**
 * Roda uma consulta de todas as categorias de produto e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void cat_prod_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar categorias de produto e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da categoria de produto selecionada.
 */
int cat_prod_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_CATEGORIA_PRODUTO_H

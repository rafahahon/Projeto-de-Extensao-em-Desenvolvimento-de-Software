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
 * Biblioteca Code Cat Coffee - Módulo Produtos
 */

#ifndef CODE_CAT_COFFEE_PRODUTO_H
#define CODE_CAT_COFFEE_PRODUTO_H

/**
 * Roda uma consulta de busca de produtos disponíveis em estoque e imprime o resultado da busca.
 */
void produto_buscar();

/**
 * Cadastra um novo produto no banco de dados.
 */
void produto_cadastrar();

/**
 * Roda uma consulta de todos os produtos disponíveis em estoque e lista os resultados.
 */
void produto_listar();

/**
 * Dá ao usuário a opção de buscar ou listar produtos disponíveis em estoque e pede para o usuário
 * selecionar um por ID.
 * @return ID, nome, preço e quantidade em estoque do produto selecionado.
 */
int produto_selecionar();

#endif //CODE_CAT_COFFEE_PRODUTO_H

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

/**
 * Roda uma consulta de busca de clientes e imprime o resultado da busca.
 */
void cliente_buscar();

/**
 * Roda uma consulta de todos os clientes e lista os resultados.
 */
void cliente_listar();

/**
 * Dá ao usuário a opção de buscar ou listar clientes e pede para o usuário selecionar um por ID.
 * @return ID do cliente selecionado.
 */
int cliente_selecionar();

#endif //CODE_CAT_COFFEE_CLIENTE_H

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
 * Biblioteca Code Cat Coffee - Módulo Reservas de Mesa
 */

#ifndef CODE_CAT_COFFEE_RESERVA_MESA_H
#define CODE_CAT_COFFEE_RESERVA_MESA_H

#include "sqlite3.h"

/**
 * Roda uma consulta de busca de reserva de mesa por nome de cliente e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_buscar_por_cliente(sqlite3* bd);

/**
 * Roda uma consulta de busca de reserva de mesa pelo número da mesa e imprime o resultado da busca.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_buscar_por_mesa(sqlite3* bd);

/**
 * Cadastra uma nova reserva de mesa.
 * @param bd A referência à conexão do banco de dados.
 * @return O ID da reserva de mesa se sucesso, 0 se falha ou erro.
 */
sqlite3_int64 reserva_mesa_cadastrar(sqlite3* bd);

/**
 * Edita uma reserva de mesa existente.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_editar(sqlite3* bd);

/**
 * Roda uma consulta de todas as reservas de mesa e lista os resultados.
 * @param bd A referência à conexão do banco de dados.
 */
void reserva_mesa_listar(sqlite3* bd);

/**
 * Dá ao usuário a opção de buscar ou listar reservas de mesa e pede para o usuário selecionar um por ID.
 * @param bd A referência à conexão do banco de dados.
 * @return ID da reserva de mesa selecionada.
 */
int reserva_mesa_selecionar(sqlite3* bd);

#endif //CODE_CAT_COFFEE_RESERVA_MESA_H

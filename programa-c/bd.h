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
 * Biblioteca Code Cat Coffee - Banco de Dados
 */

#ifndef CODE_CAT_COFFEE_BD_H
#define CODE_CAT_COFFEE_BD_H

#include "sqlite3.h"

/**
 * Conecta com o banco de dados SQLite para uso durante a execução.
 */
void bd_conectar();

/**
 * Inicializa o Banco de Dados com a estrutura básica de tabelas para o programa.
 */
void bd_criar_tabelas();

/**
 * Roda uma consulta select e imprime os nomes das colunas e cada linha no resultado.
 * @param stmt A consulta preparada (prepared statement)
 */
void bd_imprimir_resultados_tabela(sqlite3_stmt* stmt);

/**
 * Limpa e fecha tudo relativo a Banco de Dados antes de finalizar
 */
void bd_limpa_fecha();

/**
 * Preenche as tabelas do Banco de Dados com dados de exemplo.
 */
void bd_popular_tabelas();

/**
 * Cria uma prepared statement para a consulta SQL passada.
 * @param query A consulta SQL para ser preparada.
 * @return 0 se sucesso ao preparar a consulta, caso contrário, 1
 */
int bd_prepara_consulta(char* query);

/**
 * Verifica a integridade do Banco de Dados através das tabelas presentes.
 */
void bd_verificar_integridade();

#endif //CODE_CAT_COFFEE_BD_H

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
 * @return A conexão do banco de dados criada.
 */
sqlite3* bd_conectar();

/**
 * Inicializa o Banco de Dados com a estrutura básica de tabelas para o programa.
 * @param bd A referência à conexão do banco de dados.
 */
void bd_criar_tabelas(sqlite3* bd);

/**
 * Roda uma consulta select e imprime os nomes das colunas e cada linha no resultado.
 * @param bd_statement A consulta preparada (prepared statement)
 */
void bd_imprimir_resultados_tabela(sqlite3_stmt* bd_statement);

/**
 * Preenche as tabelas do Banco de Dados com dados de exemplo.
 * @param bd A referência à conexão do banco de dados.
 */
void bd_popular_tabelas(sqlite3* bd);

/**
 * Cria uma prepared statement para a consulta SQL passada.
 * @param bd A referência à conexão do banco de dados.
 * @param query A consulta SQL para ser preparada.
 * @param bd_statement A consulta preparada a ser estruturada.
 * @return 0 se sucesso ao preparar a consulta, caso contrário, 1
 */
int bd_prepara_consulta(sqlite3* bd, const char* query, sqlite3_stmt** bd_statement);

/**
 * Verifica a integridade do Banco de Dados através das tabelas presentes.
 * @param bd A referência à conexão do banco de dados.
 */
void bd_verificar_integridade(sqlite3* bd);

#endif //CODE_CAT_COFFEE_BD_H

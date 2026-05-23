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
 * Biblioteca Code Cat Coffee - Utilidades
 */

#ifndef CODE_CAT_COFFEE_UTILIDADES_H
#define CODE_CAT_COFFEE_UTILIDADES_H

#include <corecrt.h>

/**
 * Pega entrada do usuário e trata como data no formato DD-MM-YYYY.
 * Caso usuário não forneça uma data, usar a data atual.
 * @param data A variável que vai receber a entrada do usuário em stdin.
 */
void entrada_data(int* data);

/**
 * Pega entrada do usuário e trata como float.
 * @return Float capturado pelo input em stdin
 */
float entrada_float();

/**
 * Pega entrada do usuário e trata como int.
 * @return Integer capturado pelo input em stdin
 */
int entrada_int();

/**
 *
 * Pega entrada do usuário e trata como string, colocando o seu conteúdo em variavel.
 * @param variavel A variável que vai receber a entrada do usuário em stdin
 * @param tamanho_max O tamanho máximo da string
 */
void entrada_string(char* variavel, size_t tamanho_max);

/**
 * Lê um arquivo no sistema de arquivos atual e retorna o seu conteúdo.
 * @param caminho_arquivo O caminho para o arquivo a ser lido.
 * @return O conteúdo do arquivo.
 */
char* ler_arquivo(const char* caminho_arquivo);

/**
 * Valida uma variável string.
 * @param variavel A variável string a ser validada.
 * @param tamanho_min O tamanho mínimo da variável string.
 * @param obrigatorio 1 para obrigatória, 0 para opcional.
 * @return Booleano se a variável passou pela validação ou não.
 */
int valida_string(const char* variavel, int tamanho_min, int obrigatorio);

#endif //CODE_CAT_COFFEE_UTILIDADES_H

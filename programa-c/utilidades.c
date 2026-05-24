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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utilidades.h"

/**
 * Conta a quantidade real de caracteres visíveis em uma string UTF-8.
 * Ignora os bytes de continuação dos acentos.
 * @param texto O texto em UTF-8 a ter os caracteres visualmente contados.
 * @return O tamanho de caracteres visuais no texto.
 */
int tamanho_visual_utf8(const char* texto)
{
    int tamanho = 0;

    while (*texto)
    {
        // Se os dois primeiros bits não forem '10' (0x80), é o início de um caractere visual
        if ((*texto & 0xC0) != 0x80)
        {
            tamanho++;
        }
        texto++;
    }

    return tamanho;
}

/**
 * Pega entrada do usuário e trata como data no formato DD-MM-YYYY.
 * Caso usuário não forneça uma data, usar a data atual.
 * @param data A variável que vai receber a entrada do usuário em stdin.
 */
void entrada_data(int* data)
{
    char temp_var[11], *ptr, *endptr;
    time_t agora = time(NULL);

    // Obtém a data vinda do usuário via stdin
    entrada_string(temp_var, sizeof(temp_var));

    if (strcmp(temp_var, "\n") == 0) // Se a data for vazia, usar a data de hoje
    {
        strftime(temp_var, sizeof temp_var, "%d-%m-%Y", localtime(&agora));
    }

    ptr = (char*)temp_var;

    // Extrai o DIA
    const long temp_dia = strtol(ptr, &endptr, 10);

    if (ptr == endptr || *endptr != '-')
    {
        printf("Erro ao ler o dia ou delimitador '-' ausente.\n");
        return;
    }

    data[0] = (int)temp_dia;
    ptr = endptr + 1; // Avança o ponteiro para pular o hífen '-'

    // Extrai o MÊS
    const long temp_mes = strtol(ptr, &endptr, 10);

    if (ptr == endptr || *endptr != '-')
    {
        printf("Erro ao ler o mês ou delimitador '-' ausente.\n");
        return;
    }

    data[1] = (int)temp_mes;
    ptr = endptr + 1; // Avança o ponteiro para pular o segundo hífen '-'

    // Extrai o ANO
    const long temp_ano = strtol(ptr, &endptr, 10);

    // O ano deve terminar com o fim da string ('\0') ou uma nova linha ('\n')
    if (ptr == endptr || (*endptr != '\0' && *endptr != '\n'))
    {
        printf("Erro ao ler o ano ou caracteres extras no fim da string.\n");
        return;
    }

    data[2] = (int)temp_ano;
}

/**
 * Pega entrada do usuário e trata como float.
 * @return Float capturado pelo input em stdin
 */
float entrada_float()
{
    char input_buffer[128],
         *endptr;
    float temp_var = 0.0f;

    if (fgets(input_buffer, sizeof(input_buffer), stdin))
    {
        // Remove o \n antes de medir o tamanho
        input_buffer[strcspn(input_buffer, "\n")] = 0;

        // Se usuário não digitar nada, retornar −1.0f
        if (strlen(input_buffer) == 0)
        {
            return -1.0f;
        }

        // strtof converte string para float
        temp_var = strtof(input_buffer, &endptr);

        // Verificação de erro: se endptr apontar para o início, nenhum número foi encontrado
        if (input_buffer == endptr)
        {
            printf("Aviso: Número digitado inválido. Definindo como 0.0\n");
            temp_var = 0.0f;
        }
    }

    return temp_var;
}

/**
 * Pega entrada do usuário e trata como int.
 * @return Integer capturado pelo input em stdin
 */
int entrada_int()
{
    char input_buffer[128],
         *endptr;
    long temp_var = 0;

    if (fgets(input_buffer, sizeof(input_buffer), stdin))
    {
        // Remove o \n antes de medir o tamanho
        input_buffer[strcspn(input_buffer, "\n")] = 0;

        // Se usuário não digitar nada, retornar −1
        if (strlen(input_buffer) == 0)
        {
            return -1;
        }

        // strtol converte uma string em long, depois forçamos para int
        temp_var = strtol(input_buffer, &endptr, 10);

        if (input_buffer == endptr)
        {
            printf("Aviso: Valor inválido. Definindo para 0.\n");
            temp_var = 0;
        }
    }

    return (int)temp_var;
}

/**
 *
 * Pega entrada do usuário e trata como string, colocando o seu conteúdo em variavel.
 * @param variavel A variável que vai receber a entrada do usuário em stdin
 * @param tamanho_max O tamanho máximo da string
 */
void entrada_string(char* variavel, const size_t tamanho_max)
{
    char input_buffer[255];

    if (fgets(input_buffer, sizeof(input_buffer), stdin))
    {
        // Remove quebra de linha no final
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        // Copia seguramente para a nossa variável
        strncpy(variavel, input_buffer, tamanho_max - 1);
        // Reforça que a string termine com NULL
        variavel[tamanho_max - 1] = '\0';
    }
}

/**
 * Lê um arquivo no sistema de arquivos atual e retorna o seu conteúdo.
 * @param caminho_arquivo O caminho para o arquivo a ser lido.
 * @return O conteúdo do arquivo.
 */
char* ler_arquivo(const char* caminho_arquivo)
{
    printf("Lendo arquivo %s\n", caminho_arquivo);
    char* conteudo;
    // Aqui abrimos o arquivo para leitura
    FILE* arquivo = fopen(caminho_arquivo, "rb");

    if (arquivo == NULL)
    {
        printf("Não foi possível abrir o arquivo.\n");
        fclose(arquivo);
        exit(1);
    }

    // Aqui pegamos o tamanho total do arquivo para usar em seguida
    fseek(arquivo, 0, SEEK_END);
    const long tam_arquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    conteudo = malloc(tam_arquivo + 1);

    if (conteudo == NULL)
    {
        printf("Erro de falta de memória.\n");
        fclose(arquivo);
        exit(1);
    }

    // Aqui colocamos o conteúdo de cada linha do arquivo dentro da variável conteudo
    size_t bytes_lidos = fread(conteudo, 1, tam_arquivo, arquivo);
    conteudo[bytes_lidos] = '\0'; // Finaliza a string com um null

    // Fechando o arquivo para liberar memória
    fclose(arquivo);

    return conteudo;
}

/**
 * Valida uma variável string.
 * @param variavel A variável string a ser validada.
 * @param tamanho_min O tamanho mínimo da variável string.
 * @param obrigatorio 1 para obrigatória, 0 para opcional.
 * @return Booleano se a variável passou pela validação ou não.
 */
int valida_string(const char* variavel, const int tamanho_min, const int obrigatorio)
{
    // Se não for obrigatória, não precisamos validar o tamanho mínimo
    if (obrigatorio == 0)
    {
        return 1;
    }

    // Se o tamanho da string for maior ou igual ao mínimo, retorna como verdadeiro
    if (strlen(variavel) >= tamanho_min)
    {
        return 1;
    }

    return 0;
}

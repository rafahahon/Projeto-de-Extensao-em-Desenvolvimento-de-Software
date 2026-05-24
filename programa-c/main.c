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
 * Olá, pro! O nosso programa em C está ligado ao nosso banco de dados!
 */

#include <locale.h>
#include <stdio.h>
#include "bd.h"
#include "menu.h"
#include "utilidades.h"

// Para poder rodar os printf em português corretamente
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Para poder usar o encoding e o idioma português no terminal do Windows
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Aqui declaramos as opções do Menu Principal, a primeira sempre deve ser o voltar/sair
    const char* menu_principal[] = {
        "Sair",
        "Pedidos",
        "Clientes",
        "Produtos",
        "Mesas",
        "Playground",
        // "Relatórios",
        "Outros"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu_principal) / sizeof(menu_principal[0]);
    int opcao;
    sqlite3* bd;

    imprime_logo_ccc();

    bd = bd_conectar();

    // TODO: login de funcionario
    // TODO: menu por funcionario via permissoes

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("   ▓   ▓ ▓▓▓▓▓ ▓   ▓ ▓   ▓    ▓▓▓▓  ▓▓▓▓  ▓▓▓ ▓   ▓  ▓▓▓  ▓▓▓ ▓▓▓▓   ▓▓▓  ▓\n");
        printf("   ▓▓ ▓▓ ▓     ▓▓  ▓ ▓   ▓    ▓   ▓ ▓   ▓  ▓  ▓▓  ▓ ▓      ▓  ▓   ▓ ▓   ▓ ▓\n");
        printf("   ▓ ▓ ▓ ▓▓▓▓  ▓ ▓ ▓ ▓   ▓    ▓▓▓▓  ▓▓▓▓   ▓  ▓ ▓ ▓ ▓      ▓  ▓▓▓▓  ▓▓▓▓▓ ▓\n");
        printf("   ▓   ▓ ▓     ▓  ▓▓ ▓   ▓    ▓     ▓  ▓   ▓  ▓  ▓▓ ▓      ▓  ▓     ▓   ▓ ▓\n");
        printf("   ▓   ▓ ▓▓▓▓▓ ▓   ▓  ▓▓▓     ▓     ▓   ▓ ▓▓▓ ▓   ▓  ▓▓▓  ▓▓▓ ▓     ▓   ▓ ▓▓▓▓▓\n");
        imprime_linha_separadora();
        imprime_opcoes(menu_principal, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();

        switch (opcao)
        {
        case 1:
            menu_pedido(bd);
            break;

        case 2:
            menu_cliente(bd);
            break;

        case 3:
            menu_produto(bd);
            break;

        case 4:
            menu_mesa(bd);
            break;

        case 5:
            menu_playground(bd);
            break;

        case 6:
        //     menu_relatorio(bd);
        //     break;
        //
        // case 7:
            menu_outros(bd);
            break;

        case 0:
            printf("\n\nTchau tchau!\n");
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);

    sqlite3_free(NULL);
    sqlite3_close(bd);

    return 0;
}

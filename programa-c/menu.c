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
 * Biblioteca Code Cat Coffee - Menu
 */

#include <stdio.h>
#include "cliente.h"
#include "categoria_produto.h"
#include "forma_pagamento.h"
#include "funcionario.h"
#include "menu.h"
#include "mesa.h"
#include "pedido.h"
#include "produto.h"
#include "utilidades.h"

/**
 * Imprime uma linha separadora com espaçamento de uma quebra de linha acima e abaixo dela.
 */
void imprime_linha_separadora()
{
    printf("\n===================================================================================\n");
}

/**
 * Imprime o nome e a logo da Code Cat Coffee no terminal.
 */
void imprime_logo_ccc()
{
    printf("\n\n");
    printf("                         ::-======+.:*                      \n");
    printf("                    :=======%%==:%%%%=======.+                         ███   ███  ████  █████\n");
    printf("                 .=====-===%%@%%==%%===%%====-===                      █     █   █ █   █ █\n");
    printf("              .===%%====%%%%%%===========%%@=%%==%%===:                   █     █   █ █   █ ████\n");
    printf("            :=====%%==%%====%%%%#######%%%%====%%%%=%%=%%==:                 █     █   █ █   █ █\n");
    printf("           :==%%==%%====%%##-%%#########:*##%%======-%%==                 ███   ███  ████  █████\n");
    printf("          :=%%%%-=%%===####.+==.:...:#-=*#####==:-=%%===.       \n");
    printf("         .=%%===#==%%#####%%:+++*+-++++=*######%%==#%%%%=%%=.      \n");
    printf("        :========#######.++-=++++*.*++.#######==%%=====              ███   ███  █████\n");
    printf("        ==%%==@==%%#######++++.+:.+.++.+=#######%%==*==%%=:            █     █   █   █\n");
    printf("       :=======@########..+..%%%%:%%.:.+-.##.#####*=======            █     █████   █\n");
    printf("       .===+===%%#######=#.=%%%%%%%%%%%%%%%%+=:####.####%%===+===:           █     █   █   █\n");
    printf("       :++%%%%++=%%########.+-=#%%####+=+-:%%*-*%%:##%%=+%%%%%%+=.            ███  █   █   █\n");
    printf("       :+++%%%%+-%%#################.%%+++.#%%#%%#:+:%%=+%%%%=+=:    \n");
    printf("       :=++++++=#:################%%%%*+=%%#.%%##:#=++++++=     \n");
    printf(
        "        =++++++-%%#*#+##.%%##*%%.:###%%%%+*++*+-+.#%%-++++++:             ███   ███  █████ █████ █████ █████ \n");
    printf(
        "        -=++++++-%%.###############:%%++=::.@##%%=+=@%%++:             █     █   █ █     █     █     █    \n");
    printf(
        "         .=++@%%-++=++#############%%: :* +=++=++%%+@++=              █     █   █ ████  ████  ████  ████  \n");
    printf(
        "          +=++#%%%%++=:*++++#.:.-=+##*+:=+++=-+++%%%%++.               █     █   █ █     █     █     █  \n");
    printf(
        "            :=++-+*=++==+++++++++===+++==+++=+%%++=:                 ███   ███  █     █     █████ █████ \n");
    printf("              .=++#++=%%+++=====+=-===+++%%%%@%%=++=:           \n");
    printf("                :=++++++++%%%%%%+=-+@%%%%+++%%-++++=:             \n");
    printf("                   :==++++++@=%%+=+*++++++=+:                \n");
    printf("                       .:+==-+++++===+:-                    \n");
    printf("\n\n");
}

/**
 * Imprime a lista de opções de um menu. Por último sempre vai colocar a opção 0 - Voltar/Sair.
 * @param opcoes Um array de strings com as opções. A primeira sempre deve ser o Voltar/Sair.
 * @param quantidade_opcoes O total de opções disponíveis na array. Deve ser calculado junto da declaração das opções.
 */
void imprime_opcoes(const char** opcoes, const int quantidade_opcoes)
{
    printf("\n");

    for (int i = 1; i < quantidade_opcoes; i++)
    {
        printf("%d - %s\n", i, opcoes[i]);
    }

    printf("%d - %s\n", 0, opcoes[0]);
    printf("\n");
}

/**
 * Imprime o menu Clientes e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_cliente(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Clientes
    const char* menu[] = {
        "Voltar",
        "Novo Cliente",
        "Alterar Cliente",
        "Buscar Clientes",
        "Listar Clientes"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("         ###  #     ### ##### #   # ##### #####  #### \n");
        printf("        #     #      #  #     ##  #   #   #     #     \n");
        printf("        #     #      #  ####  # # #   #   ####   ###  \n");
        printf("        #     #      #  #     #  ##   #   #         # \n");
        printf("         ###  ##### ### ##### #   #   #   ##### ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            cliente_cadastrar(bd);
            break;

        case 2:
            cliente_editar(bd);
            break;

        case 3:
            cliente_buscar(bd);
            break;

        case 4:
            cliente_listar(bd);
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Mesa e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_mesa(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Mesas
    const char* menu[] = {
        "Voltar",
        "Nova Mesa",
        "Alterar Mesa",
        "Buscar Mesas",
        "Listar Mesas",
        "Nova Reserva de Mesa",
        "Alterar Reserva de Mesa",
        "Buscar Reserva de Mesas",
        "Listar Reserva de Mesas"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");

        imprime_linha_separadora();
        printf("\n");
        printf("        #   # #####  ####  ###   #### \n");
        printf("        ## ## #     #     #   # #     \n");
        printf("        # # # ####   ###  #####  ###  \n");
        printf("        #   # #         # #   #     # \n");
        printf("        #   # ##### ####  #   # ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            mesa_cadastrar(bd);
            break;

        case 2:
            mesa_editar(bd);
            break;

        case 3:
            mesa_buscar(bd);
            break;

        case 4:
            mesa_listar(bd);
            break;

        case 5:
            // TODO: criar reserva de mesa
            break;

        case 6:
            // TODO: editar reserva de mesa
            break;

        case 7:
            // TODO: buscar reserva de mesa
            break;

        case 8:
            // TODO: listar reserva de mesas
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Outros e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_outros(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Outros
    const char* menu[] = {
        "Voltar",
        "Novo Funcionário",
        "Alterar Funcionário",
        "Buscar Funcionários",
        "Listar Funcionários",
        "Nova Forma de Pagamento",
        "Alterar Forma de Pagamento",
        "Buscar Forma de Pagamento",
        "Listar Forma de Pagamento"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("         ###  #   # ##### ####   ###   #### \n");
        printf("        #   # #   #   #   #   # #   # #     \n");
        printf("        #   # #   #   #   ####  #   #  ###  \n");
        printf("        #   # #   #   #   #  #  #   #     # \n");
        printf("         ###   ###    #   #   #  ###  ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            funcionario_cadastrar(bd);
            break;

        case 2:
            funcionario_editar(bd);
            break;

        case 3:
            funcionario_buscar(bd);
            break;

        case 4:
            funcionario_listar(bd);
            break;

        case 5:
            forma_pagto_cadastrar(bd);
            break;

        case 6:
            forma_pagto_editar(bd);
            break;

        case 7:
            forma_pagto_buscar(bd);
            break;

        case 8:
            forma_pagto_listar(bd);
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Pedidos e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_pedido(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Pedidos
    const char* menu[] = {
        "Voltar",
        "Novo Pedido",
        "Alterar Pedido",
        "Buscar Pedido",
        "Listar Pedidos"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("        ####  ##### ####  ### ####   ###   #### \n");
        printf("        #   # #     #   #  #  #   # #   # #     \n");
        printf("        ####  ####  #   #  #  #   # #   #  ###  \n");
        printf("        #     #     #   #  #  #   # #   #     # \n");
        printf("        #     ##### ####  ### ####   ###  ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            pedido_criar(bd);
            break;

        case 2:
            pedido_editar(bd);
            break;

        case 3:
            pedido_buscar_por_cliente(bd);
            break;

        case 4:
            pedido_listar(bd);
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Playgrounds e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_playground(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Playgrounds
    const char* menu[] = {
        "Voltar",
        "Novo Playground",
        "Alterar Playground",
        "Buscar Playground",
        "Listar Playgrounds",
        "Novo Gato",
        "Alterar Gato",
        "Buscar Gatos",
        "Listar Gatos",
        "Nova Sessão de Playground",
        "Alterar Sessão de Playground",
        "Buscar Sessão de Playground",
        "Listar Sessões de Playground"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("        ####  #      ###  #   #  ###  ####   ###  #   # #   # ####  \n");
        printf("        #   # #     #   #  # #  #     #   # #   # #   # ##  # #   # \n");
        printf("        ####  #     #####   #   #  ## ####  #   # #   # # # # #   # \n");
        printf("        #     #     #   #   #   #   # #  #  #   # #   # #  ## #   # \n");
        printf("        #     ##### #   #   #    ###  #   #  ###   ###  #   # ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            // TODO: criar playground
            break;

        case 2:
            // TODO: editar playground
            break;

        case 3:
            // TODO: buscar playground
            break;

        case 4:
            // TODO: listar playgrounds
            break;

        case 5:
            // TODO: criar gato
            break;

        case 6:
            // TODO: editar gato
            break;

        case 7:
            // TODO: buscar gato
            break;

        case 8:
            // TODO: listar gatos
            break;

        case 9:
            // TODO: criar sessao de playground
            break;

        case 10:
            // TODO: editar sessao de playground
            break;

        case 11:
            // TODO: buscar sessao de playground
            break;

        case 12:
            // TODO: listar sessoes de playground
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Produtos e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_produto(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Produtos
    const char* menu[] = {
        "Voltar",
        "Novo Produto",
        "Alterar Produto",
        "Buscar Produtos",
        "Listar Produtos",
        "Nova Categoria de Produto",
        "Alterar Categoria de Produto",
        "Buscar Categoria de Produtos",
        "Listar Categoria de Produtos",
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("        ####  ####   ###  ####  #   # #####  ###   #### \n");
        printf("        #   # #   # #   # #   # #   #   #   #   # #     \n");
        printf("        ####  ####  #   # #   # #   #   #   #   #  ###  \n");
        printf("        #     #  #  #   # #   # #   #   #   #   #     # \n");
        printf("        #     #   #  ###  ####   ###    #    ###  ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            produto_cadastrar(bd);
            break;

        case 2:
            produto_editar(bd);
            break;

        case 3:
            produto_buscar(bd);
            break;

        case 4:
            produto_listar(bd);
            break;

        case 5:
            cat_prod_cadastrar(bd);
            break;

        case 6:
            cat_prod_editar(bd);
            break;

        case 7:
            cat_prod_buscar(bd);
            break;

        case 8:
            cat_prod_listar(bd);
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

/**
 * Imprime o menu Relatórios e as suas opções.
 * @param bd A referência à conexão do banco de dados.
 */
void menu_relatorio(sqlite3* bd)
{
    // Aqui declaramos as opções do Menu Relatórios
    const char* menu[] = {
        "Voltar",
        "Relatórios de Vendas - Pendente"
    };
    // Descobre a quantidade de itens dividindo o tamanho total da array
    // pelo tamanho de um único item (para não precisar colocar o número fixo no for)
    const int quantidade_itens_menu = sizeof(menu) / sizeof(menu[0]);
    int opcao;

    do
    {
        printf("\n\n");
        imprime_linha_separadora();
        printf("\n");
        printf("        ####  ##### #      ###  #####  ###  ####  ###  ###   #### \n");
        printf("        #   # #     #     #   #   #   #   # #   #  #  #   # #     \n");
        printf("        ####  ####  #     #####   #   #   # ####   #  #   #  ###  \n");
        printf("        #  #  #     #     #   #   #   #   # #  #   #  #   #     # \n");
        printf("        #   # ##### ##### #   #   #    ###  #   # ###  ###  ####  \n");
        imprime_linha_separadora();
        imprime_opcoes(menu, quantidade_itens_menu);
        printf("Escolha: ");
        opcao = entrada_int();


        switch (opcao)
        {
        case 1:
            // TODO: quais relatorios criar?
            break;

        case 0:
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    while (opcao != 0);
}

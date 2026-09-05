#include <stdio.h>
#include <stdlib.h>
#include "include/modelos.h"
#include "include/clientes.h"
#include "include/locacoes.h"
#include "include/relatorios.h"
#include "include/vendas.h"
#include "include/veiculos.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// =======================================================
//                  PROGRAMA PRINCIPAL-
// =======================================================

int main(void) {
    int capacidadeClientes = 2;
    int totalClientes = 0;
    Cliente *clientes = malloc(capacidadeClientes * sizeof(Cliente));

    int capacidadeVeiculos = 2;
    int totalVeiculos = 0;
    Veiculo *veiculos = malloc(capacidadeVeiculos * sizeof(Veiculo));

    int capacidadeVendas = 2;
    int totalVendas = 0;
    Venda *vendas = malloc(capacidadeVendas * sizeof(Venda));

    int capacidadeLocacoes = 2;
    int totalLocacoes = 0;
    Locacao *locacoes = malloc(capacidadeLocacoes * sizeof(Locacao));

    if (clientes == NULL || veiculos == NULL ||
        vendas == NULL || locacoes == NULL) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria.\n");
        free(clientes);
        free(veiculos);
        free(vendas);
        free(locacoes);
        return 1;
    }

    int opcao;

    do {
        printf("\n====================================");
        printf("\n|      SISTEMA DA CONCESSIONARIA   |");
        printf("\n====================================");
        printf("\n| 1 - Clientes                     |");
        printf("\n| 2 - Veiculos                     |");
        printf("\n| 3 - Vendas                       |");
        printf("\n| 4 - Locacoes                     |");
        printf("\n| 5 - Relatorio Financeiro         |");
        printf("\n| 0 - Sair                         |");
        printf("\n====================================");
        printf("\nDigite a opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            menuClientes(&clientes, &totalClientes, &capacidadeClientes,
                         vendas, totalVendas, locacoes, totalLocacoes);
        } else if (opcao == 2) {
            menuVeiculos(&veiculos, &totalVeiculos, &capacidadeVeiculos);
        } else if (opcao == 3) {
            menuVendas(&veiculos, &totalVeiculos, &clientes, &totalClientes,
                       &capacidadeClientes, &vendas, &totalVendas,
                       &capacidadeVendas);
        } else if (opcao == 4) {
            menuLocacoes(&veiculos, totalVeiculos, &clientes, totalClientes,
                         &locacoes, &totalLocacoes, &capacidadeLocacoes);
        } else if (opcao == 5) {
            exibirRelatorioFinanceiro(vendas, totalVendas,
                                      locacoes, totalLocacoes);
        }
    } while (opcao != 0);

    free(clientes);
    free(veiculos);
    free(vendas);
    free(locacoes);

    printf("\nSistema encerrado com sucesso.\n");
    return 0;
}
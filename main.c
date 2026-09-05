#include <stdio.h>
#include <stdlib.h>
#include "../include/modelos.h"
#include "../include/clientes.h"
#include "../include/locacoes.h"
#include "../include/relatorio.h"
#include "../include/vendas.h"
#include "../include/veiculos.h"


// =======================================================
//                  PROGRAMA PRINCIPAL
// =======================================================

int main() {
    int capClientes = 2, totalClientes = 0;
    Cliente *clientes = (Cliente *) malloc(capClientes * sizeof(Cliente));

    int capVeiculos = 2, totalVeiculos = 0;
    Veiculo *veiculos = (Veiculo *) malloc(capVeiculos * sizeof(Veiculo));

    int capVendas = 2, totalVendas = 0;
    Venda *vendas = (Venda *) malloc(capVendas * sizeof(Venda));

    int capLocacoes = 2, totalLocacoes = 0;
    Locacao *locacoes = (Locacao *) malloc(capLocacoes * sizeof(Locacao));

    int opcao;

    do {
        printf("\n====================================");
        printf("\n|      SISTEMA DA CONCESSIONARIA   |");
        printf("\n====================================");
        printf("\n|     1 - Clientes                 |");
        printf("\n|     2 - Veiculos                 |");
        printf("\n|     3 - Vendas                   |");
        printf("\n|     4 - Locacoes                 |");
        printf("\n|     5 - Relatorio Financeiro     |");
        printf("\n|     0 - Sair                     |");
        printf("\n====================================");
        printf("\nDigite a opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            menuClientes(&clientes, &totalClientes, &capClientes, vendas, totalVendas, locacoes, totalLocacoes);
        } else if (opcao == 2) {
            menuVeiculos(&veiculos, &totalVeiculos, &capVeiculos);
        } else if (opcao == 3) {
            menuVendas(&veiculos, &totalVeiculos, &clientes, &totalClientes, &capClientes, &vendas, &totalVendas, &capVendas);
        } else if (opcao == 4) {
            menuLocacoes(&veiculos, totalVeiculos, &clientes, totalClientes, &locacoes, &totalLocacoes, &capLocacoes);
        } else if (opcao == 5) {
            exibirRelatorioFinanceiro(vendas, totalVendas, locacoes, totalLocacoes);
        }

    } while (opcao != 0);

    free(clientes);
    free(veiculos);
    free(vendas);
    free(locacoes);

    printf("\nSistema encerrado com sucesso.\n");
    return 0;
}